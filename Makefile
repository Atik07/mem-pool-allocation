I = bin/intermediates
B = bin
S = src

all:   $(B)/unit_test 

$(B)/unit_test: $(I)/main.o $(I)/libmavalloc.a
	gcc -o $(B)/unit_test $(I)/main.o -L $(I) -lmavalloc

$(I)/main.o: $(S)/main.c
	gcc -O -c $(S)/main.c -o $(I)/main.o

$(I)/mavalloc.o: $(S)/mavalloc.c
	gcc -O -c $(S)/mavalloc.c -o $(I)/mavalloc.o

$(I)/libmavalloc.a: $(I)/mavalloc.o
	ar rcs $(I)/libmavalloc.a $(I)/mavalloc.o

clean:
	rm -f $(I)/*.o $(I)/*.a $(B)/unit_test

.PHONY: all clean
