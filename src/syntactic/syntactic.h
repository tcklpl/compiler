#pragma once
#include "../lexical/lexical.h"
#include "../logger.h"
#include <deque>
#include <algorithm>
#include <map>
#include <functional>

class SyntacticAnalyzer {

    private:
        size_t token_buffer_size = 10;
        std::shared_ptr<Logger> logger;
        std::shared_ptr<LexicalAnalyzer> lexicalAnalyzer;
        std::deque<std::shared_ptr<Token>> token_buffer;
        bool readed_everything = false;

        void error(std::vector<std::string> expcted);
        void read_token();
        std::shared_ptr<Token> look_ahead(int k);
        void match(TokenType type);
        void multi_match(std::vector<TokenType> types);

        // IMPLEMENTED IN main_semantic_rules.cpp
        void check_program();
        void check_declarations();
        void check_declaration();
        void check_var_type();

        // IMPLEMENTED IN arithmetic_semantic_rules.cpp
        void check_arithmetic_expression();
        void check_arithmetic_expression_sub();
        void check_arithmetic_expression_sub_rule1();
        void check_arithmetic_term();
        void check_arithmetic_term_sub();
        void check_arithmetic_term_sub_rule1();
        void check_arithmetic_factor();

        // IMPLEMENTED IN relational_semantic_rules.cpp
        void check_relational_expr();
        void check_relational_expr_sub();
        void check_relational_term();
        void check_relational_op();

        void check_boolean_op();

        // IMPLEMENTED IN command_semantic_rules.cpp

        std::map<TokenType, std::function<void()>> command_check_functions {
            {TokenType::res_Atribuir, std::bind(&SyntacticAnalyzer::check_command_attr, this)},
            {TokenType::res_Ler, std::bind(&SyntacticAnalyzer::check_command_input, this)},
            {TokenType::res_Imprimir, std::bind(&SyntacticAnalyzer::check_command_output, this)},
            {TokenType::res_Se, std::bind(&SyntacticAnalyzer::check_command_condition, this)},
            {TokenType::res_Enquanto, std::bind(&SyntacticAnalyzer::check_command_loop, this)},
            {TokenType::res_Inicio, std::bind(&SyntacticAnalyzer::check_sub_algorithm, this)},
        };

        void check_command_list();
        void check_command_list_rule1();
        void check_command();
        void check_command_attr();
        void check_command_input();
        void check_command_output();
        void check_command_condition();
        void check_command_condition_rule1();
        void check_command_loop();
        void check_sub_algorithm();

    public:
        SyntacticAnalyzer(std::shared_ptr<LexicalAnalyzer> la, std::shared_ptr<Logger> lg);

        void analyze();

};