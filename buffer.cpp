#include "buffer.h"
#include <string.h>
#include <algorithm>

#include <iostream>
using namespace std;

extern programstate state;

string peek(int n) {
   std::stringstream res;

   int bs = getBufferSize();

   if (n > bs) {
      for (int i = 0; i < n-bs && !(state.fh.eof()); i++) {
         char c = state.fh.get();
         if ((int)c == -1) {
            break;
         }
         state.buffer << c;
      }
   }

   bs = getBufferSize();

   int n2read = std::min(n,bs);
   //char* data = new char[n2read+1];
   //state.buffer.seekg(0);
   //state.buffer.read(data, n2read);
   //data[n2read] = '\0';
   res << state.buffer.str().substr(0,n2read);

   return res.str();
}

int getBufferSize() {
   int origpos = state.buffer.tellg();
   state.buffer.seekg(0, ios::end);
   int res = state.buffer.tellg();
   state.buffer.seekg(origpos);
   return res;
}

string _getAndClearBuffer() {
   string v = state.buffer.str();
   state.buffer.clear();
   state.buffer.str(string());
   return v;
}

string consume(int n) {
   int numToRead = n - getBufferSize();
   if (numToRead >= 0) {
      if (numToRead > 0) {
         peek(numToRead);
      }
      return _getAndClearBuffer();
   }

   string bufval = state.buffer.str();
   string retval = bufval.substr(0, n);
   string newbufval = bufval.substr(n, bufval.length()-n);
   state.buffer.str(newbufval);
   state.buffer.seekg(0, ios::beg);
   state.buffer.seekp(0, ios::end);
   return retval;
}

static bool expect(const char* str) {
   string s (str);
   int n = s.size();
   if (peek(n).compare(s) == 0) {
      return true;
   }
   else {
      return false;
   }
}

void rexpect(const char* str) {
   if (!expect(str)) {
      std::cerr << "(fail) EXPECTING " << str << "!" << std::endl;
   }
   else {
      consume(strlen(str));
   }
}
