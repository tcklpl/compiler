#include "../syntactic.h"

// programa : ':' 'DECLARACOES' listaDeclaracoes ':' 'ALGORITMO' listaComandos;
void SyntacticAnalyzer::check_program() {
    match(TokenType::lng_Delim);
    match(TokenType::res_Declaracoes);
    check_declarations();
    match(TokenType::lng_Delim);
    match(TokenType::res_Algoritmo);
    check_command_list();
    match(TokenType::lng_Fim);
}

// listaDeclaracoes : declaracao listaDeclaracoes | declaracao;
void SyntacticAnalyzer::check_declarations() {
    if (look_ahead(4).get()->type() == TokenType::lng_Delim) {
        check_declaration();
    } else if (look_ahead(4).get()->type() == TokenType::lng_Var) {
        check_declaration();
        check_declarations();
    } else {
        error(std::vector<std::string> { token_type_str[TokenType::lng_Delim], token_type_str[TokenType::lng_Var] });
    }
}

// declaracao : VARIAVEL ':' tipoVar;
void SyntacticAnalyzer::check_declaration() {
    match(TokenType::lng_Var);
    match(TokenType::lng_Delim);
    check_var_type();
}

// tipoVar : 'INTEIRO' | 'REAL';
void SyntacticAnalyzer::check_var_type() {
    multi_match(std::vector<TokenType> { TokenType::res_Inteiro, TokenType::res_Real });
}
