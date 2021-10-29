#include "../syntactic.h"

//expressaoRelacional : expressaoRelacional operadorBooleano termoRelacional | termoRelacional;
// Precisa eliminar a recursão à esquerda
// expressaoRelacional : termoRelacional expressaoRelacional2;
void SyntacticAnalyzer::check_relational_expr() {
    check_relational_term();
    check_relational_expr_sub();
}

// expressaoRelacional2 : operadorBooleano termoRelacional expressaoRelacional2 | <<vazio>>
void SyntacticAnalyzer::check_relational_expr_sub() {
    if (look_ahead(1).get()->type() == TokenType::bol_And || look_ahead(1).get()->type() == TokenType::bol_Or) {
        check_boolean_op();
        check_relational_term();
        check_relational_expr_sub();
    }
    // else vazio?
}

//termoRelacional : expressaoAritmetica OP_REL expressaoAritmetica; GRAMÁTICA ALTERADA PARA FACILITAR
void SyntacticAnalyzer::check_relational_term() {
    TokenType next = look_ahead(1).get()->type();
    if (next == TokenType::lng_Int || next == TokenType::lng_Real || next == TokenType::lng_Var || next == TokenType::lng_oParenthesis) {
        check_arithmetic_expression();
        check_relational_op();
        check_arithmetic_expression();
    } else {
        error(std::vector<std::string> { 
            token_type_str[TokenType::lng_Int], token_type_str[TokenType::lng_Real], token_type_str[TokenType::lng_Var], token_type_str[TokenType::lng_oParenthesis] 
        });
    }
}

//operadorRelacional : "<>" | "=" | ">" | ">=" | "<" | "<="
void SyntacticAnalyzer::check_relational_op() {
    multi_match(std::vector<TokenType> { 
        TokenType::rel_Dif, TokenType::res_Equals, TokenType::rel_More, TokenType::rel_MoreEqual, TokenType::rel_Less, rel_LessEqual 
    });
}

//operadorBooleano : 'E' | 'OU';
void SyntacticAnalyzer::check_boolean_op() {
    multi_match(std::vector<TokenType> { TokenType::bol_And, TokenType::bol_Or });
}
