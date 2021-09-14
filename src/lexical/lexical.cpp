#include "lexical.h"

LexicalAnalyzer::LexicalAnalyzer(std::shared_ptr<FileReader> file_reader) : freader(file_reader) {

}

std::shared_ptr<Token> LexicalAnalyzer::next_token() {
    std::shared_ptr<Token> res = nullptr;

    skip_blank_spaces_and_comments();
    freader.get()->commit_pointer();

    for (size_t i = 0; i < lexical_tests.size(); i++) {
        res = lexical_tests[i]();
        if (res == nullptr) {
            // se for o último teste, não é pra zerar o ponteiro, pois precisamos mostrar onde foi o erro
            if (i != (lexical_tests.size() - 1))
                freader.get()->reset_pointer();
        } else {
            freader.get()->commit_pointer();
            return res;
        }
    }

    throw "Lexical error: " + freader.get()->cur_lexeme();
}

void LexicalAnalyzer::skip_blank_spaces_and_comments() {
    bool comment = false;
    char c = freader.get()->read();

    while (c == ' ' || c == '\n') {
        c = freader.get()->read();
    }

    if (c == '%')
        comment = true;
    
    if (!comment)
        freader.get()->unread();
    
    while (comment) {
        
        c = freader.get()->read();

        if (c == '\n' && comment)
            comment = false;
    }
}

std::shared_ptr<Token> LexicalAnalyzer::check_for_eof() {
    char c = freader.get()->read();
    if (c == -1)
        return std::shared_ptr<Token>(new Token(TokenType::lng_Fim, "Fim"));
    return nullptr;
}

std::shared_ptr<Token> LexicalAnalyzer::check_for_keyword() {
    while (true) {
        char c = freader.get()->read();
        if (!is_uppercase_letter(c)) {
            freader.get()->unread();
            std::string lexeme = freader.get()->cur_lexeme();
            try {
                TokenType tt = keyword_tokens.at(lexeme);
                return std::shared_ptr<Token>(new Token(tt, lexeme));
            } catch (...) {
                return nullptr;
            }
        }
    }
}

std::shared_ptr<Token> LexicalAnalyzer::check_for_varible() {
    char c = freader.get()->read();
    
    if (is_lowercase_letter(c)) {
        while (true) {
            c = freader.get()->read();

            if (!is_alphanumeric(c)) {
                freader.get()->unread();
                return std::shared_ptr<Token>(new Token(TokenType::lng_Var, freader.get()->cur_lexeme()));
            }
        }
    }
    return nullptr;
}

std::shared_ptr<Token> LexicalAnalyzer::check_for_number() {
    NumberState state = NumberState::IDLE;

    while (true) {
        char c = freader.get()->read();

        switch (state) {
            case NumberState::IDLE:
                if (c == '+' || c == '-')
                    state = NumberState::SIGNAL;
                else if (is_number(c))
                    state = NumberState::READING_INT;
                else
                    return nullptr;
                break;
            case NumberState::SIGNAL:
                if (is_number(c))
                    state = NumberState::READING_INT;
                else
                    return nullptr;
                break;
            case NumberState::READING_INT:
                if (c == '.')
                    state = NumberState::SEPARATOR;
                else if (!is_number(c)) {
                    freader.get()->unread();
                    return std::shared_ptr<Token>(new Token(TokenType::lng_Int, freader.get()->cur_lexeme()));
                } else
                    return nullptr;
                break;
            case NumberState::SEPARATOR:
                if (is_number(c))
                    state = NumberState::READING_FLOAT;
                else
                    return nullptr;
                break;
            case NumberState::READING_FLOAT:
                if (!is_number(c)) {
                    freader.get()->unread();
                    return std::shared_ptr<Token>(new Token(TokenType::lng_Real, freader.get()->cur_lexeme()));
                } else
                    return nullptr;
                break;
        } 
    }
    
    return nullptr;
}

std::shared_ptr<Token> LexicalAnalyzer::check_for_operator() {
    char c = freader.get()->read();
    char n = freader.get()->read();
    std::string lexeme = std::string() + c + n;

    try {
        TokenType tt = operator_tokens.at(lexeme);
        return std::shared_ptr<Token>(new Token(tt, lexeme));
    } catch (...) {}

    lexeme.pop_back();

    try {
        TokenType tt = operator_tokens.at(lexeme);
        // tem que voltar pois o operadoe é um símbolo só
        freader.get()->unread();
        return std::shared_ptr<Token>(new Token(tt, std::string() + c));
    } catch (...) {}

    return nullptr;
}

std::shared_ptr<Token> LexicalAnalyzer::check_for_delim() {
    char c = freader.get()->read();

    if (c == ':')  
        return std::shared_ptr<Token>(new Token(TokenType::lng_Delim, ":"));
    else
        return nullptr;
}

std::shared_ptr<Token> LexicalAnalyzer::check_for_string() {
    StringState state = StringState::IDLE;
    
    while(true) {
        char c = freader.get()->read();

        switch (state) {
            case StringState::IDLE:
                if (c == '\'')
                    state = StringState::READING_STR;
                else
                    return nullptr;
                
            case StringState::READING_STR:
                if (c == '\n')
                    return nullptr;
                if (c == '\'')
                    return std::shared_ptr<Token>(new Token(TokenType::lng_Cadeia, freader.get()->cur_lexeme()));
                if (c == '\\')
                    state = StringState::NEXT_CHAR_ESCAPED;
                break;

            case StringState::NEXT_CHAR_ESCAPED:
                if (c == '\n')
                    return nullptr;
                else
                    state = StringState::READING_STR;
                break;
        }
    }
}