
CFLAGS = -std=c99 -I. -Wall

all: clean test
	@:

test: $(SRC)
	$(CC) $^ test.c $(CFLAGS) -o test-progress
	./test-progress

clean:
	rm -f test-progress


.PHONY: clean test
