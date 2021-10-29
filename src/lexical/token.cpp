#include "token.h"

Token::Token(TokenType type, std::string info, struct TokenFilePosInfo file_pos_info) : token_type(type), token_info(info), file_pos(file_pos_info) {

}