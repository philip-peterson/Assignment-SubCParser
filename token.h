#ifndef _H__TOKEN
#define _H__TOKEN

#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
using std::string;
using std::vector;
using namespace std;

static map<string, string> patterns_names;
static vector<string> tids_names;
static vector<string> patterns;
static map<int, string> tids_patterns;
static map<string, int> names_tids;

static bool patterns_dirty = false;

void register_token(string pattern, string name);

string name_from_tid(int tid);
string pattern_from_name(string name);

static bool compareViaLength(string a, string b);

string figure_token();

struct Token {
   string tokname;
   string value;
   public:
      Token(string tokname, string value) : tokname(tokname), value(value) {}
      Token(string tokname) : tokname(tokname), value() {}
};

void initialize_tokens();
static Token consume_string();
Token consume_token(string tokname);

void whites();





#endif
