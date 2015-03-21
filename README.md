Background 
=========

This was an assignment I completed for Programming Language Concepts class.
The goal of the assignment was to implement a recursive descent parser for
a fictional programming language called SubC, which is somewhat similar to C.
The output of the program is an abstract syntax tree (AST) for the input program.

The grammar which we were given can be found in `specs/project_grammar.txt`.
Sample programs can be found in `tiny_test_programs`

Building
========

Simply run `make`.

Invocation
=======

To invoke the program (which is output as an executable called `p1`), use the following form:

```
p1 -ast FILE
```

where FILE is the path to a SubC program. The output will be printed to standard out.
Note that `p1`, by specification, produces no output when the `-ast` flag is not specified.

Verification
============
To verify the correctness of a tree, compare the output of `p1` with the corresponding
`.tree` file in `tiny_test_programs`.

Sample Output
=============
```
$> make
g++ -c main.cpp -g -std=c++0x
main.cpp:343:1: warning: control may reach end of non-void function [-Wreturn-type]
}
^
1 warning generated.
g++ -c tree.cpp -g -std=c++0x
g++ -c token.cpp -g -std=c++0x
g++ -c buffer.cpp -g -std=c++0x
g++ -o p1 main.o tree.o token.o buffer.o -g -std=c++0x -g

$> ./p1 -ast tiny_test_progs/tiny_01
program(7)
. <identifier>(1)
. . factors(0)
. consts(0)
. types(0)
. dclns(1)
. . var(2)
. . . <identifier>(1)
. . . . i(0)
. . . <identifier>(1)
. . . . integer(0)
. subprogs(1)
. . fcn(8)
. . . <identifier>(1)
. . . . Factor(0)
. . . params(1)
. . . . var(2)
. . . . . <identifier>(1)
. . . . . . i(0)
. . . . . <identifier>(1)
. . . . . . integer(0)
. . . <identifier>(1)
. . . . integer(0)
. . . consts(0)
. . . types(0)
. . . dclns(1)
. . . . var(2)
. . . . . <identifier>(1)
. . . . . . j(0)
. . . . . <identifier>(1)
. . . . . . integer(0)
. . . block(1)
. . . . if(2)
. . . . . >(2)
. . . . . . <identifier>(1)
. . . . . . . i(0)
. . . . . . <integer>(1)
. . . . . . . 0(0)
. . . . . for(4)
. . . . . . assign(2)
. . . . . . . <identifier>(1)
. . . . . . . . j(0)
. . . . . . . <integer>(1)
. . . . . . . . 1(0)
. . . . . . <=(2)
. . . . . . . <identifier>(1)
. . . . . . . . j(0)
. . . . . . . <identifier>(1)
. . . . . . . . i(0)
. . . . . . assign(2)
. . . . . . . <identifier>(1)
. . . . . . . . j(0)
. . . . . . . +(2)
. . . . . . . . <identifier>(1)
. . . . . . . . . j(0)
. . . . . . . . <integer>(1)
. . . . . . . . . 1(0)
. . . . . . if(2)
. . . . . . . =(2)
. . . . . . . . mod(2)
. . . . . . . . . <identifier>(1)
. . . . . . . . . . i(0)
. . . . . . . . . <identifier>(1)
. . . . . . . . . . j(0)
. . . . . . . . <integer>(1)
. . . . . . . . . 0(0)
. . . . . . . output(1)
. . . . . . . . integer(1)
. . . . . . . . . <identifier>(1)
. . . . . . . . . . j(0)
. . . <identifier>(1)
. . . . Factor(0)
. block(1)
. . repeat(3)
. . . read(1)
. . . . <identifier>(1)
. . . . . i(0)
. . . assign(2)
. . . . <identifier>(1)
. . . . . d(0)
. . . . call(2)
. . . . . <identifier>(1)
. . . . . . Factor(0)
. . . . . <identifier>(1)
. . . . . . i(0)
. . . <=(2)
. . . . <identifier>(1)
. . . . . i(0)
. . . . <integer>(1)
. . . . . 0(0)
. <identifier>(1)
. . factors(0)
```
