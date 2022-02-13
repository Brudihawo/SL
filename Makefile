CFLAGS:= -Wall -Werror --pedantic --std=c11

.PHONY: test

.DEFAULT_GOAL = test

test:
	$(CC) -o test test.c $(CFLAGS)

compiledb:
	compiledb make -Bnwk test

clean:
	rm -vfr **/*.o
	rm -vfr *.o
	rm -vf test
