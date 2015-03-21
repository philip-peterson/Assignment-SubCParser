#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>

#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "tree.h"
#include "main.h"
#include "programstate.h"
#include "token.h"
#include "buffer.h"

programstate state;

#define NEXTIS(X) (tcanpeek(tokens) && tpeek(tokens).tokname.compare( #X ) == 0)

using std::string;
using std::ios;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

bool tcanpeek(vector<Token> *tokens) {
   return tokens->size() != 0;
}

Token tpeek(vector<Token> *tokens) {
   if (tokens->size() == 0) {
      cerr << "Unexpected end." << endl;
      exit(87);
   }
   Token k = (*tokens)[0];
   return k;
}

Token lookahead(vector<Token> *tokens, int howmany) {
   if (tokens->size() < howmany-1) {
      return Token("<OUTOFRANGE>", "<OUTOFRANGE>");
   }
   return (*tokens)[howmany];
}

Token tpop(vector<Token> *tokens) {
   Token k = tpeek(tokens);
   tokens->erase(tokens->begin());
   return k;
}

void texpect(string tokname, vector<Token> *tokens) {
   if (tpop(tokens).tokname.compare(tokname) != 0) {
      cerr << "Expected token type " << tokname << ". Exiting." << endl;
      exit(88);
   }
}


void texpecting(string tokname, vector<Token> *tokens) {
   if (tpeek(tokens).tokname.compare(tokname) != 0) {
      cerr << "Expecting token type " << tokname << ". Exiting." << endl;
      exit(89);
   }
}


Tree *doStringNode(vector<Token> *tokens) {
   Tree* t = new stringLeafTree(nullptr);
   texpecting("T_STRING", tokens);
   t->singleValue = tpop(tokens).value;
   return t;
}

Tree *doChar(vector<Token> *tokens) {
   Tree* t = new charLeafTree(nullptr);
   texpecting("T_CHAR", tokens);
   t->singleValue = tpop(tokens).value;
   return t;
}

Tree *doInteger(vector<Token> *tokens) {
   Tree* t = new integerLeafTree(nullptr);
   texpecting("T_INTEGER", tokens);
   t->singleValue = tpop(tokens).value;
   return t;
}

Tree *doName(vector<Token> *tokens) {
   Tree* t = new identifierLeafTree(nullptr);
   texpecting("T_IDENTIFIER", tokens);
   t->singleValue = tpop(tokens).value;
   return t;
}

Tree *doPrimary(vector<Token> *tokens) {
   Token a = tpeek(tokens);

   Tree* t;

   if (a.tokname.compare("T_MINUS") == 0) {
      Token temp = tpop(tokens);
      t = new minusTree(nullptr);
      Tree *s = doPrimary(tokens);
      addChild(t, s);
   }
   else if (a.tokname.compare("T_PLUS") == 0) {
      tpop(tokens);
      t = doPrimary(tokens);
   }
   else if (a.tokname.compare("T_NOT") == 0) {
      tpop(tokens);
      t = new notTree(nullptr);
      Tree *s = doPrimary(tokens);
      addChild(t, s);
   }
   else if (a.tokname.compare("T_EOF") == 0) {
      tpop(tokens);
      t = new eofTree(nullptr);
   }
   else if (a.tokname.compare("T_INTEGER") == 0) {
      t = new integerLeafTree(nullptr);
      t->singleValue = tpop(tokens).value;
   }
   else if (a.tokname.compare("T_CHAR") == 0) {
      t = new charLeafTree(nullptr);
      t->singleValue = tpop(tokens).value;
   }
   else if (a.tokname.compare("T_LPAREN") == 0) {
      tpop(tokens);
      t = doExpression(tokens);
      texpect("T_RPAREN", tokens);
   }
   else if (a.tokname.compare("T_SUCC") == 0) {
      tpop(tokens);
      texpect("T_LPAREN", tokens);

      t = new succTree(nullptr);
      Tree *s = doExpression(tokens);
      texpect("T_RPAREN", tokens);
      addChild(t, s);
   }
   else if (a.tokname.compare("T_PRED") == 0) {
      tpop(tokens);
      texpect("T_LPAREN", tokens);

      t = new predTree(nullptr);
      Tree *s = doExpression(tokens);
      addChild(t, s);

      texpect("T_RPAREN", tokens);
   }
   else if (a.tokname.compare("T_CHR") == 0) {
      tpop(tokens);
      texpect("T_LPAREN", tokens);

      t = new chrTree(nullptr);
      Tree *s = doExpression(tokens);
      addChild(t, s);

      texpect("T_RPAREN", tokens);
   }
   else if (a.tokname.compare("T_ORD") == 0) {
      tpop(tokens);
      texpect("T_LPAREN", tokens);

      t = new ordTree(nullptr);
      Tree *s = doExpression(tokens);
      addChild(t, s);

      texpect("T_RPAREN", tokens);
   }
   else {

      Tree *name = doName(tokens);
      if (tcanpeek(tokens) && NEXTIS(T_LPAREN)) {
         // -> Name '(' Expression list ',' ')'      => "call"
         t = new callTree(nullptr);
         addChild(t, name);
         texpect("T_LPAREN", tokens);

         if (NEXTIS(T_RPAREN)) {
            // no args
         }
         else {
            while(true) {
               Tree *s = doExpression(tokens);
               addChild(t, s);
               if (NEXTIS(T_COMMA)) {
                  tpop(tokens);
               }
               else {
                  break;
               }
            }
         }

         texpect("T_RPAREN", tokens);
      }
      else {
         // -> Name
         t = name;
      }
   }
   
   return t;
}

Tree *doFactor(vector<Token> *tokens) {
   Tree *t;
   Tree *prim1 = doPrimary(tokens);

   bool justPrimary = true;
   if (tcanpeek(tokens)) {
      justPrimary = false;
      if (NEXTIS(T_MULTIPLY)) {
         t = new timesTree(nullptr);
      }
      else if (NEXTIS(T_DIVIDE)) {
         t = new slashTree(nullptr);
      }
      else if (NEXTIS(T_AND)) {
         t = new andTree(nullptr);
      }
      else if (NEXTIS(T_MOD)) {
         t = new modTree(nullptr);
      }
      else {
         justPrimary = true;
      }
   }

   if (justPrimary) {
      return prim1;
   }

   tpop(tokens);
   addChild(t, prim1);
   addChild(t, doPrimary(tokens));
   // TODO fix associativity
   return t;
}

Tree *doTerm(vector<Token> *tokens) {
   // Possible problem: order of production rules
   // is not correct
   // 
   // check correctness
   Tree *t;
   Tree *fac1 = doFactor(tokens);

   bool justFactor = true;
   if (tcanpeek(tokens)) {
      justFactor = false;
      if (NEXTIS(T_PLUS)) {
         t = new plusTree(nullptr);
      }
      else if (NEXTIS(T_MINUS)) {
         t = new minusTree(nullptr);
      }
      else if (NEXTIS(T_OR)) {
         t = new orTree(nullptr);
      }
      else {
         justFactor = true;
      }
   }

   if (justFactor) {
      return fac1;
   }

   tpop(tokens);
   addChild(t, fac1);
   addChild(t, doTerm(tokens));
   // TODO fix associativity
   return t;
}

Tree *doExpression(vector<Token> *tokens) {
   Tree *t;

   Tree *term1;
   term1 = doTerm(tokens);

   bool other = false;
   if (NEXTIS(T_LTE)) {
      t = new lteTree(nullptr);
   }
   else if (NEXTIS(T_LT)) {
      t = new ltTree(nullptr);
   }
   else if (NEXTIS(T_GTE)) {
      t = new gteTree(nullptr);
   }
   else if (NEXTIS(T_GT)) {
      t = new gtTree(nullptr);
   }
   else if (NEXTIS(T_EQ)) {
      t = new eqTree(nullptr);
   }
   else if (NEXTIS(T_NE)) {
      t = new neTree(nullptr);
   }
   else {
       // -> Term
      other = true;
   }

   if (!other) {
      addChild(t, term1);
      tpop(tokens);
      addChild(t, doTerm(tokens));
   }
   else {
      return term1;
   }

   return t;
}


Tree *doForExp(vector<Token> *tokens) {
   if (
      NEXTIS(T_MINUS)
      || NEXTIS(T_PLUS)
      || NEXTIS(T_NOT)
      || NEXTIS(T_EOF)
      || NEXTIS(T_IDENTIFIER)
      || NEXTIS(T_INTEGER)
      || NEXTIS(T_CHAR)
      || NEXTIS(T_LPAREN)
      || NEXTIS(T_SUCC)
      || NEXTIS(T_PRED)
      || NEXTIS(T_CHR)
      || NEXTIS(T_ORD)
   ) {
      return doExpression(tokens);
   }
}


Tree *doAssignment(vector<Token> *tokens) {
   Tree *n = doName(tokens);
   if (NEXTIS(T_SWAP)) {
      Tree *n2 = doName(tokens);
      Tree *t = new swapTree(nullptr);
      addChild(t, n);
      addChild(t, n2);
      return t;
   }
   texpect("T_ASSIGNMENT", tokens);

   Tree *e = doExpression(tokens);
   Tree *t = new assignTree(nullptr);
   addChild(t, n);
   addChild(t, e);
   return t;
}

Tree *doForStat(vector<Token> *tokens) {
   if ( NEXTIS(T_IDENTIFIER)) {
      return doAssignment(tokens);
   }
   Tree *t = new nullTree(nullptr);
   return t;
}

// MAY RETURN NULL
Tree *doOtherwiseClause(vector<Token> *tokens) {
   if ( NEXTIS(T_OTHERWISE)) {
      Tree *t = new otherwiseTree(nullptr);
      Tree *s = doStatement(tokens);
      addChild(t, s);
      return t;
   }
   return nullptr;
}

Tree *doCaseExpression(vector<Token> *tokens) {
   Tree *cv1 = doConstValue(tokens);
   if (NEXTIS(T_DOTS)) {
      tpop(tokens);
      Tree *cv2 = doConstValue(tokens);
      Tree *t = new dotsTree(nullptr);
      addChild(t, cv1);
      addChild(t, cv2);
      return t;
   }
   return cv1;
}

Tree *doConstValue(vector<Token> *tokens) {
   if (NEXTIS(T_INTEGER)) {
      return doInteger(tokens);
   }
   else if (NEXTIS(T_CHAR)) {
      return doChar(tokens);
   }
   else {
      return doName(tokens);
   }
}

Tree *doCaseclause(vector<Token> *tokens) {
   // does the list mean at least one? or zero?
   // assuming at least one
   Tree *t = new caseClauseTree(nullptr);


   if (
         NEXTIS(T_INTEGER)
         || NEXTIS(T_CHAR)
         || NEXTIS(T_IDENTIFIER)
      ) {
      addChild(t, doCaseExpression(tokens));
      while(NEXTIS(T_COMMA)) {
         tpop(tokens);
         addChild(t, doCaseExpression(tokens));
      }
   }

   texpect("T_COLON", tokens);
   addChild(t, doStatement(tokens));
   return t;
}

vector<Tree*> doCaseclauses(vector<Token> *tokens) {
   vector<Tree*> res;
   do {
      res.push_back(doCaseclause(tokens));
      texpect("T_SEMICOLON", tokens);
   } while (
      NEXTIS(T_INTEGER)
      || NEXTIS(T_CHAR)
      || NEXTIS(T_IDENTIFIER)
   );
   return res;
}


Tree *doOutExp(vector<Token> *tokens) {
   if(NEXTIS(T_STRING)) {
      Tree *t = new stringTree(nullptr);
      addChild(t, doStringNode(tokens));
      return t;
   }
   Tree *t = new integerTree(nullptr);
   addChild(t, doExpression(tokens));
   return t;
}

Tree *doFcn(vector<Token> *tokens) {
   Tree *t = new fcnTree(nullptr);
   texpect("T_FUNCTION", tokens);
   addChild(t, doName(tokens));
   texpect("T_LPAREN", tokens);
   addChild(t, doParams(tokens));
   texpect("T_RPAREN", tokens);
   texpect("T_COLON", tokens);
   addChild(t, doName(tokens));
   texpect("T_SEMICOLON", tokens);
   addChild(t, doConsts(tokens));
   addChild(t, doTypes(tokens));
   addChild(t, doDclns(tokens));
   addChild(t, doBody(tokens));
   addChild(t, doName(tokens));
   texpect("T_SEMICOLON", tokens);
   return t;
}

Tree *doStatement(vector<Token> *tokens) {
   if (NEXTIS(T_IDENTIFIER)) {
      return doAssignment(tokens);
   }
   if (NEXTIS(T_OUTPUT)) {
      tpop(tokens);
      texpect("T_LPAREN", tokens);
      Tree *t = new outputTree(nullptr);

      if (!NEXTIS(T_RPAREN)) {
         addChild(t, doOutExp(tokens));
         while (NEXTIS(T_COMMA)) {
            tpop(tokens);
            addChild(t, doOutExp(tokens));
         }
      }

      texpect("T_RPAREN", tokens);
      return t;
   }
   if (NEXTIS(T_IF)) {
      tpop(tokens);
      Tree *t = new ifTree(nullptr);
      addChild(t, doExpression(tokens));
      texpect("T_THEN", tokens);
      addChild(t, doStatement(tokens));

      if (tcanpeek(tokens) && NEXTIS(T_ELSE)) {
         tpop(tokens);
         addChild(t, doStatement(tokens));
      }
      return t;
   }
   if (NEXTIS(T_WHILE)) {
      tpop(tokens);
      Tree *t = new whileTree(nullptr);
      addChild(t, doExpression(tokens));
      texpect("T_DO", tokens);
      addChild(t, doStatement(tokens));
      return t;
   }
   if (NEXTIS(T_REPEAT)) {
      tpop(tokens);
      Tree *t = new repeatTree(nullptr);

      if (!NEXTIS(T_UNTIL)) {
         addChild(t, doStatement(tokens));
         while (NEXTIS(T_SEMICOLON)) {
            tpop(tokens);
            addChild(t, doStatement(tokens));
         }
      }

      texpect("T_UNTIL", tokens);
      addChild(t, doExpression(tokens));
      return t;
   }
   if (NEXTIS(T_FOR)) {
      Tree *t = new forTree(nullptr);
      texpect("T_FOR", tokens);
      texpect("T_LPAREN", tokens);

      addChild(t, doForStat(tokens));
      texpect("T_SEMICOLON", tokens);
      addChild(t, doForExp(tokens));
      texpect("T_SEMICOLON", tokens);
      addChild(t, doForStat(tokens));

      texpect("T_RPAREN", tokens);

      addChild(t, doStatement(tokens));
      return t;
   }
   if (NEXTIS(T_LOOP)) {
      Tree *t = new loopTree(nullptr);
      texpect("T_LOOP", tokens);

      if (!NEXTIS(T_POOL)) {
         addChild(t, doStatement(tokens));
         while (NEXTIS(T_SEMICOLON)) {
            tpop(tokens);
            addChild(t, doStatement(tokens));
         }
      }

      texpect("T_POOL", tokens);
      return t;
   }
   if (NEXTIS(T_CASE)) {
      Tree *t = new caseTree(nullptr);
      texpect("T_CASE", tokens);
      addChild(t, doExpression(tokens));
      texpect("T_OF", tokens);

      vector<Tree*> cc = doCaseclauses(tokens);
      for (int i = 0; i < cc.size(); i++) {
         addChild(t, cc[i]);
      }

      Tree *owc = doOtherwiseClause(tokens);
      if (owc != nullptr) {
         addChild(t, owc);
      }

      texpect("T_END", tokens);
      return t;
   }
   if (NEXTIS(T_READ)) {
      Tree *t = new readTree(nullptr);
      texpect("T_READ", tokens);
      texpect("T_LPAREN", tokens);

      if (!NEXTIS(T_RPAREN)) {
         addChild(t, doName(tokens));
         while (NEXTIS(T_COMMA)) {
            tpop(tokens);
            addChild(t, doName(tokens));
         }
      }

      texpect("T_RPAREN", tokens);
      return t;
   }
   if (NEXTIS(T_EXIT)) {
      return new exitTree(nullptr);
   }
   if (NEXTIS(T_RETURN)) {
      tpop(tokens);
      Tree *t = new returnTree(nullptr);
      addChild(t, doExpression(tokens));
      return t;
   }
   if (NEXTIS(T_BEGIN)) {
      return doBody(tokens);
   }
   return new nullTree(nullptr);
}

Tree *doBody(vector<Token> *tokens) {
   texpect("T_BEGIN", tokens);
   Tree *t = new blockTree(nullptr);


   if (!NEXTIS(T_END)) {
      addChild(t, doStatement(tokens));
      //cout << "TPEEK:" << tpeek(tokens).tokname << endl;
      while (NEXTIS(T_SEMICOLON)) {
         tpop(tokens);
         addChild(t, doStatement(tokens));
      }
   }

   texpect("T_END", tokens);

   return t;
}

Tree *doDcln(vector<Token> *tokens) {
   Tree *t = new varTree(nullptr);


   if (!NEXTIS(T_COLON)) {
      addChild(t, doName(tokens));
      while (NEXTIS(T_COMMA)) {
         tpop(tokens);
         addChild(t, doName(tokens));
      }
   }

   texpect("T_COLON", tokens);
   addChild(t, doName(tokens));

   return t;
}

Tree *doDclns(vector<Token> *tokens) {
   Tree *t = new dclnsTree(nullptr);

   if (NEXTIS(T_VAR)) {
      tpop(tokens);
      do  {
         addChild(t, doDcln(tokens));
         texpect("T_SEMICOLON", tokens);
      } while (NEXTIS(T_IDENTIFIER));
   }

   return t;
}

Tree *doParams(vector<Token> *tokens) {
   Tree *t = new paramsTree(nullptr);

   // XXX this one allows zero
   if (NEXTIS(T_IDENTIFIER)) {
      addChild(t, doDcln(tokens));
      while (NEXTIS(T_SEMICOLON)) {
         tpop(tokens);
         addChild(t, doDcln(tokens));
      }
   }
   return t;
}


Tree *doTiny(vector<Token> *tokens) {
   Tree *t = new programTree(nullptr);

   texpect("T_PROGRAM", tokens);
   addChild(t, doName(tokens));

   texpect("T_COLON", tokens);
   addChild(t, doConsts(tokens));
   addChild(t, doTypes(tokens));
   addChild(t, doDclns(tokens));
   addChild(t, doSubProgs(tokens));
   addChild(t, doBody(tokens));
   addChild(t, doName(tokens));

   texpect("T_DOT", tokens);

   return t;
}

Tree *doSubProgs(vector<Token> *tokens) {
   Tree *t = new subprogsTree(nullptr);

   while (NEXTIS(T_FUNCTION)) {
      addChild(t, doFcn(tokens));
   }

   return t;
}

Tree *doConsts(vector<Token> *tokens) {
   Tree *t = new constsTree(nullptr);

   if (NEXTIS(T_CONST)) {
      tpop(tokens);
      // XXX this one allows zero
      if (NEXTIS(T_IDENTIFIER)) {
         addChild(t, doConst(tokens));
         while (NEXTIS(T_COMMA)) {
            tpop(tokens);
            addChild(t, doConst(tokens));
         }
         texpect("T_SEMICOLON", tokens);
      }
   }

   return t;
}

Tree *doConst(vector<Token> *tokens) {
   Tree *t = new constTree(nullptr);

   addChild(t, doName(tokens));
   texpect("T_EQ", tokens);
   addChild(t, doConstValue(tokens));

   return t;
}

Tree *doType(vector<Token> *tokens) {
   Tree *t = new typeTree(nullptr);

   addChild(t, doName(tokens));
   texpect("T_EQ", tokens);
   addChild(t, doLitList(tokens));

   return t;
}

Tree *doLitList(vector<Token> *tokens) {
   Tree *t = new litTree(nullptr);

   texpect("T_LPAREN", tokens);

   // XXX this one allows zero
   if (NEXTIS(T_IDENTIFIER)) {
      addChild(t, doName(tokens));
      while (NEXTIS(T_COMMA)) {
         tpop(tokens);
         addChild(t, doName(tokens));
      }
   }

   texpect("T_RPAREN", tokens);

   return t;
}

// TODO fix all (...)+ so they actually require at least one

Tree *doTypes(vector<Token> *tokens) {
   Tree *t = new typesTree(nullptr);

   if (NEXTIS(T_TYPE)) {
      tpop(tokens);
      do {
         addChild(t, doType(tokens));
         texpect("T_SEMICOLON", tokens);
      } while (NEXTIS(T_IDENTIFIER));
   }

   return t;
}

int main(int argc, char** argv) {
   initialize_tokens();
   if (argc < 3) {
      return 0;
   }
   if (strcmp(argv[1], "-ast") != 0) {
      return 1;
   }

   state.fh.open(argv[2], std::ifstream::in);
   std::vector<Token> tokens;
   while (true) {
      string tokname = figure_token();
      if (tokname.compare("<INVALID_TOK>") == 0) {
         cerr << "Invalid token. Exiting." << endl;
         return 800;
      }
      else if (tokname.compare("<END>") == 0) {
         break;
      }
      Token mytok = consume_token(tokname);
      tokens.push_back(mytok);
   }

   // Remove newlines
   
   int i = 0;
   while(true) {
      if (i >= tokens.size()) {
         break;
      }
      if (tokens[i].tokname.compare("T_NEWLINE") == 0) {
         tokens.erase(tokens.begin()+i);
      }
      else {
         i++;
      }
   }

   // end remove newlines

   //for (int j = 0; j < tokens.size() ; j++) {
   //   cout << "token " << tokens[j].tokname << endl;
   //}

   Tree *t = doTiny(&tokens);
   printTree(*t);

   state.fh.close();
}
