#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>

#include <string.h>
#include <assert.h>

#include "tree.h"

#include "programstate.h"
#include "token.h"
#include "buffer.h"

#include <stdlib.h>

extern programstate state;

using std::string;
using std::ios;
using std::cout;
using std::endl;


void addChild(Tree *parent, Tree *child) {
   child->parent = parent;
   parent->children.push_back(child);
}

void _printTreeHelper(Tree tree, int num) {
   std::stringstream o;
   for (int i = 0; i < num; i++) {
      o << ". ";
   }
   o << tree.treePrintName;
   if (tree.hasSingleValue) {
      assert(tree.children.size() == 0);
      o << "(1)";
   }
   else {
      o << "(" << tree.children.size() << ")";
   }
   std::cout << o.str() << std::endl;

   for(int i = 0; i < tree.children.size(); i++) { // TODO optimize
      _printTreeHelper(*tree.children.at(i), num+1);
   }

   if (tree.hasSingleValue) {
      std::stringstream q;
      for (int i = 0; i < num+1; i++) {
         q << ". ";
      }
      q << tree.singleValue;
      q << "(0)";
      std::cout << q.str() << std::endl;
   }
}

void printTree(Tree tree) {
   _printTreeHelper(tree, 0);
}



//Tree buildAst() {
//   whites();
//
//   rexpect("program");
//   buildName();
//   rexpect(":");
//   buildConsts();
//   buildTypes();
//   buildDclns();
//   buildSubProgs();
//   buildBody();
//   buildName();
//   rexpect(".");
//
//   whites();
//}

// void handleIdentifier(std::vector<Token> *toks) {
//    char c;
//    std::stringstream s;
// 
//    // c is given to be a valid starting char for an ident
//    c = peek(1)[0];
//    s << c;
//    consume(1);
// 
//    while (true) {
//       c = peek(1)[0];
//       if (
//             ('a' <= c && c <= 'z')
//             || ('A' <= c && c <= 'Z')
//             || ('0' <= c && c <= '9')
//             || c == '_'
//          ) {
//          consume(1);
//          s << c;
//       }
//       else {
//          break;
//       }
//    }
//    toks->push_back(Token(T_IDENTIFIER, s.str()));
// }

// void handleInteger(std::vector<Token> *toks) {
//    char c;
//    std::stringstream s;
//    while (true) {
//       c = peek(1)[0];
//       if ('0' <= c && c <= '9') {
//          consume(1);
//          s << c;
//       }
//       else {
//          break;
//       }
//    }
//    toks->push_back(Token(T_INTEGER, s.str()));
// }

// void handleComment(std::vector<Token> *toks) {
//    rexpect("{");
//    while (peek(1).compare("}") != 0) {
//       consume(1);
//    }
//    rexpect("}");
// }
// 
// void handleString(std::vector<Token> *toks) {
//    rexpect("\"");
//    std::stringstream s;
//    char c;
//    while (true) {
//       c = peek(1)[0];
//       if (c == '"') {
//          break;
//       }
//       else if (c == '\0') {
//          std::cerr << "Unterminated string. Exiting." << std::endl;
//          exit(10);
//       }
//       else {
//          s << c;
//          consume(1);
//       }
//    }
//    rexpect("\"");
//    toks->push_back(Token(T_STRING, s.str()));
// }
// 
// void handleChar(std::vector<Token> *toks) {
//    rexpect("'");
//    std::stringstream s ( peek(1) );
//    consume(1);
//    toks->push_back(Token(T_CHAR, s.str()));
//    rexpect("'");
// }


