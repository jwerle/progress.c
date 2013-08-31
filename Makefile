
SRC = src/progress.c
CFLAGS = -std=c99 -lm -I deps -I include

all: clean test
	@:

test: $(SRC)
	$(CC) $^ test.c $(CFLAGS) -o test-progress

clean:
	rm -f test-progress


.PHONY: clean test
