// this program takes in a lemon file and writes out the mapping from them all
// to strings
package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type thingy struct {
	value int
	name  string
}

func main() {
	f, err := os.Open(os.Args[1])
	if err != nil {
		panic(err)
	}
	scanner := bufio.NewScanner(f)
	items := []thingy{}
	for scanner.Scan() {
		line := strings.Fields(scanner.Text())
		if len(line) != 3 {
			fmt.Println(line)
			break
		}
		v, _ := strconv.Atoi(line[2])
		items = append(items, thingy{v, line[1]})
	}
	f.Close()

	if len(items) == 0 {
		return
	}

	f, err = os.Create("sym.h")
	if err != nil {
		panic(err)
	}

	fmt.Fprintf(f, `#ifndef _sym_h_
#define _sym_h_

char *sym_to_string(int sym);

#endif
`)
	f.Close()

	f, err = os.Create("sym.c")
	if err != nil {
		panic(err)
	}

	fmt.Fprintf(f, `
#include <stdio.h>
#include "sym.h"

char *sym_to_string(int sym) {
	char *ret;
	switch (sym) {
`)

	for _, i := range items {
		fmt.Fprintf(f, `case %d: return "%s"; break;`, i.value, i.name)
		fmt.Fprintf(f, "\n")
	}

	fmt.Fprint(f, `
		default:
			asprintf(&ret, "sym_to_string: YOU DID A BAD-BAD: %d not a good symbol", sym);
			return ret;
	}
}`)
}
