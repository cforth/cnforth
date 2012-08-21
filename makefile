CC := gcc
CFLAGS := -Wall
cforth: cforth.o code_words.o dict.o
	$(CC) $(CFLAGS) $^ -o $@
cforth.o: cforth.c cforth.h code_words.h dict.h
	$(CC) $(CFLAGS) -c cforth.c
code_words.o: code_words.c code_words.h
	$(CC) $(CFLAGS) -c code_words.c
dict.o: dict.c dict.h
	$(CC) $(CFLAGS) -c dict.c
clean:
	@echo "cleanning project"
	-rm -f cforth *.o
	@echo "clean completed"
