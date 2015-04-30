DOCUMENTATION
=============

Input: A (hopefully) valid Pascal subset program
Output: The parse tree, the symbol table, and the results of initial semantic checks.

Build requirements:
-------------------
- clang, for specific extensions (blocks)
- a go compiler, for the program that generates sym_to_string(int)
- lemon, a parser generator
- flex, a lexer generator
- a little bit of soul, to sacrifice to the gods.

Design:
-------
lindenmayer is a multiple pass compiler.
The first pass builds a parse tree from the input.
The second pass takes the parse tree and produces a symbol table, annotating IDs in the parse tree
with the scope to start searching for the symbol in.
The third pass walks the parse tree doing semantic checks. This is in progress.
The fourth pass would generate code. That didn't happen.

Usege:
------
make
./dragon [input_file]

If invoked without an input file, lindenmayer will read from stdin.
