#pragma once
#include <memory>
#include <map>
#include <functional>
#include "../filereader.h"
#include "../char_utils.h"
#include "token.h"

enum class StringState {
    IDLE, READING_STR, NEXT_CHAR_ESCAPED
};

enum class NumberState {
    IDLE, READING_INT, SEPARATOR, READING_FLOAT
};

class LexicalAnalyzer {
    private:
        std::shared_ptr<FileReader> freader;
        std::map<std::string, TokenType> operator_tokens {
            {"*", TokenType::art_Mul},
            {"/", TokenType::art_Div},
            {"+", TokenType::art_Sum},
            {"-", TokenType::art_Sub},
            {"(", TokenType::lng_oParenthesis},
            {")", TokenType::lng_cParenthesis},
            {"=", TokenType::res_Equals},
            {"<", TokenType::rel_Less},
            {">", TokenType::rel_More},
            {"<=", TokenType::rel_LessEqual},
            {">=", TokenType::rel_MoreEqual},
            {"<>", TokenType::rel_Dif},
        };

        std::map<std::string, TokenType> keyword_tokens {
            {"DECLARACOES", TokenType::res_Declaracoes},
            {"ALGORITMO", TokenType::res_Algoritmo},
            {"INT", TokenType::res_Inteiro},
            {"REAL", TokenType::res_Real},
            {"ATRIBUIR", TokenType::res_Atribuir},
            {"A", TokenType::res_A},
            {"LER", TokenType::res_Ler},
            {"IMPRIMIR", TokenType::res_Imprimir},
            {"SE", TokenType::res_Se},
            {"ENTAO", TokenType::res_Entao},
            {"SENAO", TokenType::res_Senao},
            {"ENQUANTO", TokenType::res_Enquanto},
            {"INICIO", TokenType::res_Inicio},
            {"FIM", TokenType::res_Fim},
            {"E", TokenType::bol_And},
            {"OU", TokenType::bol_Or},
        };

        std::vector<std::function<std::shared_ptr<Token>()>> lexical_tests {
            std::bind(&LexicalAnalyzer::check_for_eof, this),
            std::bind(&LexicalAnalyzer::check_for_keyword, this),
            std::bind(&LexicalAnalyzer::check_for_varible, this),
            std::bind(&LexicalAnalyzer::check_for_number, this),
            std::bind(&LexicalAnalyzer::check_for_operator, this),
            std::bind(&LexicalAnalyzer::check_for_delim, this),
            std::bind(&LexicalAnalyzer::check_for_string, this),
        };

        struct TokenFilePosInfo get_current_file_pos();

        void skip_blank_spaces_and_comments();
        std::shared_ptr<Token> check_for_eof();
        std::shared_ptr<Token> check_for_keyword();
        std::shared_ptr<Token> check_for_varible();
        std::shared_ptr<Token> check_for_number();
        std::shared_ptr<Token> check_for_operator();
        std::shared_ptr<Token> check_for_delim();
        std::shared_ptr<Token> check_for_string();
    public:
        LexicalAnalyzer(std::shared_ptr<FileReader> file_reader);

        std::shared_ptr<Token> next_token();
};