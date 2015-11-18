CC := gcc
CFLAGS := -Wall
forth: forth.o code_words.o dict.o
	$(CC) $(CFLAGS) $^ -o $@
forth.o: forth.c forth.h code_words.h dict.h
	$(CC) $(CFLAGS) -c forth.c
code_words.o: code_words.c code_words.h
	$(CC) $(CFLAGS) -c code_words.c
dict.o: dict.c dict.h
	$(CC) $(CFLAGS) -c dict.c
clean:
	@echo "cleanning project"
	-rm -f forth *.o
	@echo "clean completed"
