#include "lexical.h"

LexicalAnalyzer::LexicalAnalyzer(std::shared_ptr<FileReader> file_reader) : freader(file_reader) {

}

struct TokenFilePosInfo LexicalAnalyzer::get_current_file_pos() {
    struct TokenFilePosInfo current = { .line = freader->cur_line() + 1, .file_pos = freader->cur_pos() };
    return current;
}

std::shared_ptr<Token> LexicalAnalyzer::next_token() {
    std::shared_ptr<Token> res = nullptr;

    skip_blank_spaces_and_comments();
    freader->commit_pointer();

    for (size_t i = 0; i < lexical_tests.size(); i++) {
        res = lexical_tests[i]();
        if (res == nullptr) {
            // se for o último teste, não é pra zerar o ponteiro, pois precisamos mostrar onde foi o erro
            if (i != (lexical_tests.size() - 1))
                freader->reset_pointer();
        } else {
            freader->commit_pointer();
            return res;
        }
    }

    throw "Lexical error!: Near '" + freader->cur_lexeme() + "', On position " + 
        std::to_string(freader->cur_pos()) + ", Line " + std::to_string(freader->cur_line() + 1);
}

void LexicalAnalyzer::skip_blank_spaces_and_comments() {
    bool comment = false;
    char c = freader->read();

    while (c == ' ' || c == '\n') {
        c = freader->read();
    }

    if (c == '%')
        comment = true;
    
    if (!comment)
        freader->unread();
    
    while (comment) {
        
        c = freader->read();

        if (c == '\n' && comment)
            comment = false;
    }
}

std::shared_ptr<Token> LexicalAnalyzer::check_for_eof() {
    char c = freader->read();
    if (c == -1)
        return std::shared_ptr<Token>(new Token(TokenType::lng_Fim, "Fim", get_current_file_pos()));
    return nullptr;
}

std::shared_ptr<Token> LexicalAnalyzer::check_for_keyword() {
    while (true) {
        char c = freader->read();
        if (!is_uppercase_letter(c)) {
            freader->unread();
            std::string lexeme = freader->cur_lexeme();
            try {
                TokenType tt = keyword_tokens.at(lexeme);
                return std::shared_ptr<Token>(new Token(tt, lexeme, get_current_file_pos()));
            } catch (...) {
                return nullptr;
            }
        }
    }
}

std::shared_ptr<Token> LexicalAnalyzer::check_for_varible() {
    char c = freader->read();
    
    if (is_lowercase_letter(c)) {
        while (true) {
            c = freader->read();

            if (!is_alphanumeric(c)) {
                freader->unread();
                return std::shared_ptr<Token>(new Token(TokenType::lng_Var, freader->cur_lexeme(), get_current_file_pos()));
            }
        }
    }
    return nullptr;
}

std::shared_ptr<Token> LexicalAnalyzer::check_for_number() {
    NumberState state = NumberState::IDLE;

    while (true) {
        char c = freader->read();

        switch (state) {
            case NumberState::IDLE:
                if (is_number(c))
                    state = NumberState::READING_INT;
                else
                    return nullptr;
                break;                
            case NumberState::READING_INT:
                if (c == '.')
                    state = NumberState::SEPARATOR;
                else if (!is_number(c)) {
                    freader->unread();
                    return std::shared_ptr<Token>(new Token(TokenType::lng_Int, freader->cur_lexeme(), get_current_file_pos()));
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
                    freader->unread();
                    return std::shared_ptr<Token>(new Token(TokenType::lng_Real, freader->cur_lexeme(), get_current_file_pos()));
                } else
                    return nullptr;
                break;
        } 
    }
    
    return nullptr;
}

std::shared_ptr<Token> LexicalAnalyzer::check_for_operator() {
    char c = freader->read();
    char n = freader->read();
    std::string lexeme = std::string() + c + n;

    try {
        TokenType tt = operator_tokens.at(lexeme);
        return std::shared_ptr<Token>(new Token(tt, lexeme, get_current_file_pos()));
    } catch (...) {}

    lexeme.pop_back();

    try {
        TokenType tt = operator_tokens.at(lexeme);
        // tem que voltar pois o operadoe é um símbolo só
        freader->unread();
        return std::shared_ptr<Token>(new Token(tt, std::string() + c, get_current_file_pos()));
    } catch (...) {}

    return nullptr;
}

std::shared_ptr<Token> LexicalAnalyzer::check_for_delim() {
    char c = freader->read();

    if (c == ':')  
        return std::shared_ptr<Token>(new Token(TokenType::lng_Delim, ":", get_current_file_pos()));
    else
        return nullptr;
}

std::shared_ptr<Token> LexicalAnalyzer::check_for_string() {
    StringState state = StringState::IDLE;
    
    while(true) {
        char c = freader->read();

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
                    return std::shared_ptr<Token>(new Token(TokenType::lng_Cadeia, freader->cur_lexeme(), get_current_file_pos()));
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