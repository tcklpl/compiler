#include "../syntactic.h"

//listaComandos : comando listaComandos | comando;
void SyntacticAnalyzer::check_command_list() {
    check_command();
    check_command_list_rule1();
}

// listaComandos : comando (listaComandos | <<vazio>>)
void SyntacticAnalyzer::check_command_list_rule1() {
    TokenType next = look_ahead(1)->type();
    if (next == TokenType::res_Atribuir || next == TokenType::res_Ler || next == TokenType::res_Imprimir || 
        next == TokenType::res_Se || next == TokenType::res_Enquanto || next == TokenType::res_Inicio) {
        check_command_list();
    }
    // else vazio
}

//comando : comandoAtribuicao | comandoEntrada | comandoSaida | comandoCondicao | comandoRepeticao | subAlgoritmo;
void SyntacticAnalyzer::check_command() {
    try {
        command_check_functions.at(look_ahead(1)->type())();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        error(std::vector<std::string> { 
            token_type_str[TokenType::res_Atribuir], 
            token_type_str[TokenType::res_Ler], 
            token_type_str[TokenType::res_Imprimir], 
            token_type_str[TokenType::res_Se], 
            token_type_str[TokenType::res_Enquanto], 
            token_type_str[TokenType::res_Inicio] 
        });
    }
}

//comandoAtribuicao : 'ATRIBUIR' expressaoAritmetica 'A' VARIAVEL;
void SyntacticAnalyzer::check_command_attr() {
    match(TokenType::res_Atribuir);
    check_arithmetic_expression();
    match(TokenType::res_A);
    match(TokenType::lng_Var);
}

//comandoEntrada : 'LER' VARIAVEL;
void SyntacticAnalyzer::check_command_input() {
    match(TokenType::res_Ler);
    match(TokenType::lng_Var);
}

//comandoSaida : 'IMPRIMIR'  (VARIAVEL | CADEIA);
void SyntacticAnalyzer::check_command_output() {
    match(TokenType::res_Imprimir);
    multi_match(std::vector<TokenType> { TokenType::lng_Var, TokenType::lng_Cadeia });
}

//comandoCondicao : 'SE' expressaoRelacional 'ENTAO' comando | 'SE' expressaoRelacional 'ENTAO' comando 'SENAO' comando;
// fatorar à esquerda
// comandoCondicao : 'SE' expressaoRelacional 'ENTAO' comando ('SENAO' comando | <<vazio>>)
void SyntacticAnalyzer::check_command_condition() {
    match(TokenType::res_Se);
    check_relational_expr();
    match(TokenType::res_Entao);
    check_command();
    check_command_condition_rule1();
}

void SyntacticAnalyzer::check_command_condition_rule1() {
    if (look_ahead(1).get()->type() == TokenType::res_Senao) {
        match(TokenType::res_Senao);
        check_command();
    }
    // else vazio
}

//comandoRepeticao : 'ENQUANTO' expressaoRelacional comando;
void SyntacticAnalyzer::check_command_loop() {
    match(TokenType::res_Enquanto);
    check_relational_expr();
    check_command();
}

// subAlgoritmo : 'INICIO' listaComandos 'FIM';
void SyntacticAnalyzer::check_sub_algorithm() {
    match(TokenType::res_Inicio);
    check_command_list();
    match(TokenType::res_Fim);
}