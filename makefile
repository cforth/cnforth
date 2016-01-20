CC := gcc
CFLAGS := -Wall
forth: forth.o
	$(CC) $(CFLAGS) $^ -o $@
forth.o: forth.c forth.h
	$(CC) $(CFLAGS) -c forth.c
clean:
	@echo "cleanning project"
	-rm -f *.o
	@echo "clean completed"
