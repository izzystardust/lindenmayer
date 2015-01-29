# Big Dragon

This is the Big Dragon Compiler - it targets a subset of Pascal.

# Dependencies

* [nex](https://crypto.stanford.edu/~blynn/nex/) - A lexical analyzer generator, like lex. Only needed if making changes to the lexer.

# Compiling

I'm writing this in Go. You'll need [the go toolchain](http://golang.org).

To build, run `go get github.com/millere/big_dragon` (assuming a `GOPATH` is set).
If changes to the lexer or parser are made, `go generate` will need to be run:
```
$ go generate
$ go build
$ ./big_dragon
```
