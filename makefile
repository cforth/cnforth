CC := gcc
CFLAGS := -Wall
forth: forth.o words.o
	$(CC) $(CFLAGS) $^ -o $@
forth.o: forth.c forth.h words.h
	$(CC) $(CFLAGS) -c forth.c
words.o: words.c words.h
	$(CC) $(CFLAGS) -c words.c
clean:
	@echo "cleanning project"
	-rm -f *.o
	@echo "clean completed"
