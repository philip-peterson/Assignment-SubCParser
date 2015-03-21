Background 
=========

This was an assignment I completed for Programming Language Concepts class.
The goal of the assignment was to implement a recursive descent parser for
a fictional programming language called SubC, which is somewhat similar to C.

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
