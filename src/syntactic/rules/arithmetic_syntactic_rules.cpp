#include "../syntactic.h"

/*
    expressaoAritmetica : 
        expressaoAritmetica '+' termoAritmetico | 
        expressaoAritmetica '-' termoAritmetico | 
        termoAritmetico;
*/
// expressaoAritmetica : termoAritmetico expressaoAritmetica2
void SyntacticAnalyzer::check_arithmetic_expression() {
    check_arithmetic_term();
    check_arithmetic_expression_sub();
}

// expressaoAritmetica2 : ('+' termoAritmetico | '-' termoAritmetico) expressaoAritmetica2 | <<vazio>>
void SyntacticAnalyzer::check_arithmetic_expression_sub() {
    if (look_ahead(1).get()->type() == TokenType::art_Sum || look_ahead(1).get()->type() == TokenType::art_Sub) {
        check_arithmetic_expression_sub_rule1();
        check_arithmetic_expression_sub();
    }
    // else vazio?
}

void SyntacticAnalyzer::check_arithmetic_expression_sub_rule1() {
    multi_match(std::vector<TokenType> { TokenType::art_Sum, TokenType::art_Sub });
    check_arithmetic_term();
}

/*
    termoAritmetico : 
        termoAritmetico '*' fatorAritmetico | 
        termoAritmetico '/' fatorAritmetico | 
        fatorAritmetico;
*/
void SyntacticAnalyzer::check_arithmetic_term() {
    check_arithmetic_factor();
    check_arithmetic_term_sub();
}

void SyntacticAnalyzer::check_arithmetic_term_sub() {
    if (look_ahead(1).get()->type() == TokenType::art_Mul || look_ahead(1).get()->type() == TokenType::art_Div) {
        check_arithmetic_term_sub_rule1();
        check_arithmetic_term_sub();
    }
    // else vazio?
}

void SyntacticAnalyzer::check_arithmetic_term_sub_rule1() {
    multi_match(std::vector<TokenType> { TokenType::art_Mul, TokenType::art_Div });
    check_arithmetic_factor();
}

// fatorAritmetico : NUMINT | NUMREAL | VARIAVEL | '(' expressaoAritmetica ')'
void SyntacticAnalyzer::check_arithmetic_factor() {
    if (look_ahead(1).get()->type() == TokenType::lng_oParenthesis) {
        match(TokenType::lng_oParenthesis);
        check_arithmetic_expression();
        match(TokenType::lng_cParenthesis);
    } else {
        multi_match(std::vector<TokenType> { TokenType::lng_Int, TokenType::lng_Real, TokenType::lng_Var });
    }
}
