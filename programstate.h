#ifndef __H_PROGSTATE
#define __H_PROGSTATE

#include <iostream>
#include <fstream>
#include <sstream>

struct programstate {
   std::ifstream fh;
   std::stringstream buffer;
};

#endif
