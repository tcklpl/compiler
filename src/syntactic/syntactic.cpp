#include "syntactic.h"

SyntacticAnalyzer::SyntacticAnalyzer(std::shared_ptr<LexicalAnalyzer> la, std::shared_ptr<Logger> lg) : logger(lg), lexicalAnalyzer(la) {
    read_token();
}

void SyntacticAnalyzer::read_token() {
    if (!token_buffer.empty()) {
        token_buffer.pop_front();
    }
    while (token_buffer.size() < token_buffer_size && !readed_everything) {
        std::shared_ptr<Token> next = lexicalAnalyzer->next_token();
        if (next->type() == TokenType::lng_Fim) {
            readed_everything = true;
        }
        token_buffer.push_back(next);
    }
    std::shared_ptr<Token> to_print = look_ahead(1);
    if (to_print != nullptr)
        logger->info("Read: " + look_ahead(1)->get_as_str_full());
}

std::shared_ptr<Token> SyntacticAnalyzer::look_ahead(int k) {
    if (token_buffer.empty()) return nullptr;
    if ((size_t) k - 1 >= token_buffer.size()) return token_buffer.back();
    return token_buffer.at(k - 1);
}

void SyntacticAnalyzer::error(std::vector<std::string> expcted) {
    std::string error_msg = "Syntax error!: expecting one of the following (";
    for (size_t i = 0; i < expcted.size(); i++) {
        error_msg += expcted.at(i) + ",";
    }
    // remove last comma
    auto nt = look_ahead(1);
    error_msg.pop_back();
    error_msg += "), but found '";
    error_msg += nt->get_as_str();
    error_msg += "', On position ";
    error_msg += std::to_string(nt->get_file_pos().file_pos);
    error_msg += ", Line ";
    error_msg += std::to_string(nt->get_file_pos().line);
    throw error_msg;
}

void SyntacticAnalyzer::match(TokenType type) {
    if (look_ahead(1).get()->type() == type) {
        logger->info("Match: " + look_ahead(1)->get_as_str_full());
        read_token();
    } else {
        error(std::vector<std::string> {token_type_str[type]});
    }
}

void SyntacticAnalyzer::multi_match(std::vector<TokenType> types) {
    if (types.empty()) return;
    TokenType target = look_ahead(1).get()->type();
    
    for (TokenType t : types) {
        if (t == target) {
            match(t);
            return;
        }
    }

    std::vector<std::string> possibilities;
    for (TokenType t : types) {
        possibilities.push_back(token_type_str[t]);
    }
    error(possibilities);
}

void SyntacticAnalyzer::analyze() {
    check_program();
}