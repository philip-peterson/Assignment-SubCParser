#ifndef _H__MAIN
#define _H__MAIN

Tree *doStringNode(vector<Token> *tokens) ;
Tree *doChar(vector<Token> *tokens) ;
Tree *doInteger(vector<Token> *tokens) ;
Tree *doName(vector<Token> *tokens) ;
Tree *doPrimary(vector<Token> *tokens) ;
Tree *doFactor(vector<Token> *tokens) ;
Tree *doTerm(vector<Token> *tokens) ;
Tree *doExpression(vector<Token> *tokens) ;
Tree *doForExp(vector<Token> *tokens) ;
Tree *doAssignment(vector<Token> *tokens) ;
Tree *doForStat(vector<Token> *tokens) ;
Tree *doOtherwiseClause(vector<Token> *tokens) ;
Tree *doCaseExpression(vector<Token> *tokens) ;
Tree *doConstValue(vector<Token> *tokens) ;
Tree *doCaseclause(vector<Token> *tokens) ;
Tree *doOutExp(vector<Token> *tokens) ;
Tree *doFcn(vector<Token> *tokens) ;
Tree *doStatement(vector<Token> *tokens) ;
Tree *doBody(vector<Token> *tokens) ;
Tree *doDcln(vector<Token> *tokens) ;
Tree *doDclns(vector<Token> *tokens) ;
Tree *doParams(vector<Token> *tokens) ;
Tree *doTiny(vector<Token> *tokens) ;
Tree *doSubProgs(vector<Token> *tokens) ;
Tree *doConsts(vector<Token> *tokens) ;
Tree *doConst(vector<Token> *tokens) ;
Tree *doType(vector<Token> *tokens) ;
Tree *doLitList(vector<Token> *tokens) ;
Tree *doTypes(vector<Token> *tokens) ;


#endif
