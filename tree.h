#ifndef _H__TREE
#define _H__TREE

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>

#include <string.h>
#include <assert.h>

#include "programstate.h"
#include "token.h"
#include "buffer.h"

#include <stdlib.h>

//programstate state;

using std::string;
using std::ios;
using std::cout;
using std::endl;

class Tree {
   public:
      string treeName;
      string treePrintName;
      string singleValue;
      bool hasSingleValue;
      Tree* parent;
      std::vector<Tree*> children;
   protected:
      Tree(
         Tree *parent
         , string treeName
         , string treePrintName
         , bool hasSingleValue
      ) : children()
          , parent(parent)
          , treeName(treeName)
          , treePrintName(treePrintName)
          , hasSingleValue(hasSingleValue)
          , singleValue("") {}
};

#define _TREE_TYPE(name, leaf, printName) class name ## Tree : public Tree { \
   public: \
      name ## Tree(Tree *parent) \
         : Tree(parent, #name, printName, leaf) {} \
};

#define LEAF_TYPE(name, printName) _TREE_TYPE(name ## Leaf, true, printName)
#define TREE_TYPE(name, printName) _TREE_TYPE(name, false, printName)

#include "treetypes.cpp"

void addChild(Tree *parent, Tree *child);

void _printTreeHelper(Tree tree, int num);

void printTree(Tree tree);

#endif
