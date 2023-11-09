CC=gcc
CFLAGS:::=-Wall -fsanitize=address
DIRS=$(dir $(wildcard ./**/))
LIB=-lpthread
OUT=$(DIRS)%.c

test:
	echo $(CFLAGS)

debug: CFLAGS+=$(CFLAGS) -g
	echo $(CFLAGS)

build: my_fifo num_rec ex_merge
	echo "hi"

my_fifo: 
	$(CC) $(CFLAGS) -o $@ $(LIB)

num_rec:
	$(CC) $(CFLAGS) -o $@ $(LIB)

ex_merge:
	$(CC) $(CFLAGS) -o $@ $(LIB)

.PHONY: clean
clean:
	rm 

