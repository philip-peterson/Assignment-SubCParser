#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>

#include "token.h"
#include "programstate.h"
#include "buffer.h"

using std::string;
using std::vector;
using std::cout;
using std::cerr;

extern programstate state;

string pattern_from_name(string name) {
   int tid = names_tids[name];
   return tids_patterns[tid];
}

void register_token(string pattern, string name) {
   patterns_dirty = true;
   static int n = 0;
   n++;
   if (pattern.compare("") != 0) {
      patterns_names[pattern] = name;
      tids_patterns[n] = pattern;
      patterns.push_back(pattern);
   }
   tids_names.push_back(name);
   names_tids[name] = n;
}

void whites() {
   string c;
   while (true) {
      c = peek(1);
      bool done = true;
      switch (c[0]) {
         case '\v':
         case '\t':
         case '\f':
         case ' ':
            consume(1);
            done = false;
         break;
      }
      if (done) {
         return;
      }
   }
}

string name_from_tid(int tid) {
   return tids_names[tid-1];
}


static bool compareViaLength(string a, string b) {
   return a.length() > b.length();
}

static Token consume_ident() {
   char c;
   std::stringstream s;

   // c is given to be a valid starting char for an ident
   c = peek(1)[0];
   s << c;
   consume(1);

   while (true) {
      c = peek(1)[0];
      if (
            ('a' <= c && c <= 'z')
            || ('A' <= c && c <= 'Z')
            || ('0' <= c && c <= '9')
            || c == '_'
         ) {
         consume(1);
         s << c;
      }
      else {
         break;
      }
   }
   return Token("T_IDENTIFIER", s.str());
}

static Token consume_integer() {
   char c;
   std::stringstream s;
   while (true) {
      c = peek(1)[0];
      if ('0' <= c && c <= '9') {
         consume(1);
         s << c;
      }
      else {
         break;
      }
   }
   return Token("T_INTEGER", s.str());
}

static Token consume_string() {
   rexpect("\"");
   std::stringstream s;
   char c;
   while (true) {
      c = peek(1)[0];
      if (c == '"') {
         break;
      }
      else if (c == '\0') {
         std::cerr << "Unterminated string. Exiting." << std::endl;
         exit(10);
      }
      else {
         s << c;
         consume(1);
      }
   }
   rexpect("\"");
   return Token("T_STRING", s.str());
}

static bool skip_comment() {
   whites();
   bool wasComment = false;
   if (peek(1)[0] == '{') {
      wasComment = true;
      consume(1);
      while(true) {
         char p = peek(1)[0];
         if (p == '\0') {
            cerr << "Unterminated comment. Quitting.";
            exit(73);
         }
         else if (p == '}') {
            consume(1);
            break;
         }
         else {
            consume(1);
         }
      }
   }
   return wasComment;
}
static bool skip_comment2() {
   whites();
   bool wasComment = false;
   if (peek(1)[0] == '#') {
      wasComment = true;
      consume(1);
      while(true) {
         char p = peek(1)[0];
         if (p == '\n') {
            break;
         }
         else {
            consume(1);
         }
      }
   }
   return wasComment;
}

static Token consume_char() {
   rexpect("'");
   std::stringstream s;
   s << "'" << peek(1) << "'";
   consume(1);
   rexpect("'");
   return Token("T_CHAR", s.str());
}

Token consume_token(string tokname) {
   whites();
   if ( tokname.compare("T_STRING") == 0) {
      return consume_string();
   }
   else if ( tokname.compare("T_CHAR") == 0) {
      return consume_char();
   }
   else if ( tokname.compare("T_INTEGER") == 0) {
      return consume_integer();
   }
   else if ( tokname.compare("T_IDENTIFIER") == 0) {
      return consume_ident();
   }
   else if ( 
      tokname.compare("<END>") == 0
   ) {
      cerr << "Fatal error. Should not have consumed <END>" << endl;
      exit(24);
   }
   else if ( 
      tokname.compare("<INVALID_TOK>") == 0
   ) {
      cerr << "Fatal error. Invalid token." << endl;
      exit(25);
   }
   else {
      string pattern = pattern_from_name(tokname);
      consume(pattern.length());
      return Token(tokname);
   }
   return Token("<!!FATAL ERROR!!>", "<!!FATAL ERROR!!>");
}


string figure_token() {
   whites();
   while ( skip_comment() || skip_comment2() ) {}

   if (patterns_dirty) {
      std::sort(patterns.begin(), patterns.end(), compareViaLength);
      patterns_dirty = false;
   }

   int maxlen = patterns[0].length();
   string p = peek(maxlen);
   int plen = p.length();

   if (plen == 0) {
      return "<END>";
   }
   //



   if (p[0] == '"') {
      return "T_STRING";
   }
   else if (p[0] == '\'') {
      return "T_CHAR";
   }
   else if ('0' <= p[0] && p[0] <= '9') {
      return "T_INTEGER";
   }

   for (int i=0; i<patterns.size(); i++) {
      string q = patterns[i];
      int qlen = q.length();
      if (qlen > plen) {
         continue;
      }
      bool matches = true;
      for (int j = 0; j < qlen; j++) {
         if (p[j] != q[j]) {
            matches = false;
            break;
         }
      }
      if (matches) {
         whites();
         return patterns_names[q];
      }
   }
   whites();

   if (
      ('a' <= p[0] && p[0] <= 'z')
      || ('A' <= p[0] && p[0] <= 'Z')
      || p[0] == '_'
   ) {
      return "T_IDENTIFIER";
   }

   return "<INVALID_TOK>";
}

void initialize_tokens() {
   register_token("\n", "T_NEWLINE");
   register_token("program", "T_PROGRAM");
   register_token("var", "T_VAR");
   register_token("const", "T_CONST");
   register_token("type", "T_TYPE");
   register_token("function", "T_FUNCTION");
   register_token("return", "T_RETURN");
   register_token("begin", "T_BEGIN");
   register_token("end", "T_END");
   register_token(":=:", "T_SWAP");
   register_token(":=", "T_ASSIGNMENT");
   register_token("output", "T_OUTPUT");
   register_token("if", "T_IF");
   register_token("then", "T_THEN");
   register_token("else", "T_ELSE");
   register_token("while", "T_WHILE");
   register_token("do", "T_DO");
   register_token("case", "T_CASE");
   register_token("of", "T_OF");
   register_token("..", "T_DOTS");
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
   register_token("*", "T_MULTIPLY");
   register_token("/", "T_DIVIDE");

   register_token("", "T_INTEGER");
   register_token("", "T_CHAR");
   register_token("", "T_STRING");
   register_token("", "T_IDENTIFIER");
}
