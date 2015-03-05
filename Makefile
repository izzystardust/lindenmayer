dragon: dragon.c parse scan
	$(CC) -g -o dragon dragon.c lemon.c scan.c -ll

parse: lemon.y
	lemon lemon.y

scan: scan.l
	flex --header-file=scan.h -o scan.c scan.l

clean:
	$(RM) *.o scan.c dragon lemon.c lemon.h scan.h
