CC      = clang
CFLAGS  = -g
LDFLAGS = -ll -ly

objects = scan.o parse.o dragon.o

dragon: $(objects)
	$(CC) -o $@ $(LDFLAGS) $^

dragon.o: dragon.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $^

scan.o: scan.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $^

scan.c: scan.l
	flex -o $@ $^

parse.o: parse.tab.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $^

parse.tab.c: parse.y
	bison -d -v $^

clean:
	$(RM) *.o scan.c dragon
