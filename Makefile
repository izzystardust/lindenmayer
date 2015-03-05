dragon: dragon.c parse scan
	$(CC) -g -o dragon dragon.c parse.tab.c scan.c -ll

parse: parse.y
	bison -d -v parse.y

scan: scan.l
	flex -o scan.c scan.l

clean:
	$(RM) *.o scan.c dragon parse.tab.*
