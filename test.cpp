#include <iostream>
#include <map>

#include "token.h"

using namespace std;

int main(int argc, char** argv) {
   register_token("\n", "T_NEWLINE");
   register_token("program", "T_PROGRAM");
   register_token("var", "T_VAR");
   register_token("const", "T_CONST");
   register_token("type", "T_TYPE");
   register_token("function", "T_FUNCTION");
   register_token("return", "T_RETURN");
   register_token("begin", "T_BEGIN");
   register_token("end", "T_END");
   register_token("swap", "T_SWAP");
   register_token("assignment", "T_ASSIGNMENT");
   register_token("output", "T_OUTPUT");
   register_token("if", "T_IF");
   register_token("then", "T_THEN");
   register_token("else", "T_ELSE");
   register_token("while", "T_WHILE");
   register_token("do", "T_DO");
   register_token("case", "T_CASE");
   register_token("dots", "T_DOTS");
   register_token("otherwise", "T_OTHERWISE");
   register_token("repeat", "T_REPEAT");
   register_token("for", "T_FOR");
   register_token("until", "T_UNTIL");
   register_token("loop", "T_LOOP");
   register_token("pool", "T_POOL");
   register_token("exit", "T_EXIT");
   register_token("<=", "T_LTE");
   register_token("<>", "T_NE");
   register_token("<", "T_LT");
   register_token(">=", "T_GTE");
   register_token(">", "T_GT");
   register_token("=", "T_EQ");
   register_token("mod", "T_MOD");
   register_token("and", "T_AND");
   register_token("or", "T_OR");
   register_token("not", "T_NOT");
   register_token("read", "T_READ");
   register_token("succ", "T_SUCC");
   register_token("pred", "T_PRED");
   register_token("chr", "T_CHR");
   register_token("ord", "T_ORD");
   register_token("eof", "T_EOF");
   register_token("{", "T_LBRACE");
   register_token("}", "T_RBRACE");
   register_token(":", "T_COLON");
   register_token(";", "T_SEMICOLON");
   register_token(".", "T_DOT");
   register_token(",", "T_COMMA");
   register_token("(", "T_LPAREN");
   register_token(")", "T_RPAREN");
   register_token("+", "T_PLUS");
   register_token("-", "T_MINUS");
   register_token("*", "T_ASTERISK");
   register_token("/", "T_SLASH");
   //
   register_token("", "T_INTEGER");
   register_token("", "T_CHAR");
   register_token("", "T_STRING");
   register_token("", "T_IDENTIFIER");
}
