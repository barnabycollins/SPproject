all: libgol.so gameoflife.c gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -o gameoflife gameoflife.c gol.c

libgol.so: gol.c gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -shared -o libgol.so -fPIC gol.c

gol.o: gol.c gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -o gol.o gol.c

clean:
	rm -rf gameoflife libgol.so gol.o