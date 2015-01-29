package main

import "os"

//go:generate nex rp.nex
//go:generate go tool yacc rp.y

func main() {
	yyParse(NewLexer(os.Stdin))
}
