#pragma once
#include <iostream>

enum TokenType {
    // REServed
    res_Declaracoes, res_Algoritmo, res_Atribuir, res_Inteiro, res_Real, res_A, res_Ler, res_Imprimir, res_Se, res_Entao, res_Senao, 
    res_Enquanto, res_Inicio, res_Fim,
    // ARiTimetic
    art_Mul, art_Div, art_Sum, art_Sub,
    // RELational
    rel_Less, rel_LessEqual, res_Equals, rel_More, rel_MoreEqual, rel_Dif,
    // BOoLean
    bol_And, bol_Or,
    // LaNGuage
    lng_Delim, lng_oParenthesis, lng_cParenthesis, lng_Var, lng_Int, lng_Real, lng_Cadeia, lng_Fim
};

static const char *token_type_str[] = {
    "res_Declaracoes", "res_Algoritmo", "res_Atribuir", "res_Inteiro", "res_Real", "res_A", "res_Ler", "res_Imprimir", "res_Se",
    "res_Entao", "res_Senao", "res_Enquanto", "res_Inicio", "res_Fim",
    "art_Mul", "art_Div", "art_Sum", "art_Sub",
    "rel_Less", "rel_LessEqual", "res_Equals", "rel_More", "rel_MoreEqual", "rel_Dif",
    "bol_And", "bol_Or",
    "lng_Delim", "lng_oParenthesis", "lng_cParenthesis", "lng_Var", "lng_Int", "lng_Real", "lng_Cadeia", "lng_Fim"
};

struct TokenFilePosInfo {
    size_t line, file_pos;
};

class Token {
    private:
        TokenType token_type;
        std::string token_info;
        struct TokenFilePosInfo file_pos;
    public:
        Token(TokenType type, std::string info, struct TokenFilePosInfo file_pos_info);

        TokenType type() const { return token_type; }
        std::string get_as_str() const { return token_type_str[token_type]; }
        std::string get_as_str_full() const { return "<" + get_as_str() + ",'" + token_info + "'>"; }
        struct TokenFilePosInfo get_file_pos() const { return file_pos; }

        friend std::ostream& operator<<(std::ostream &strm, const Token &t) {
            return strm << "<" << token_type_str[t.token_type] << ",'" << t.token_info << "'>";
        }
};