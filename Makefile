dragon: dragon.c parse scan strings
	$(CC) -g -o dragon dragon.c lemon.c scan.c union.c sym.c -ll

parse: lemon.y
	lemon lemon.y

scan: scan.l
	flex --header-file=scan.h -o scan.c scan.l

strings: parse
	go run fix.go lemon.h

clean:
	$(RM) *.o scan.c dragon lemon.c lemon.h scan.h sym.h sym.c lemon.out parse.output parse.tab.*
