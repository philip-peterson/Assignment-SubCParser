#ifndef _H__BUFFER
#define _H__BUFFER

#include <string>
using std::string;

#include <iostream>
#include <string>
#include <sstream>
#include "programstate.h"

using std::string;
using std::ios;

string peek(int n);

int getBufferSize();

string _getAndClearBuffer();

string consume(int n);

static bool expect(const char* str);
void rexpect(const char* str);

#endif
