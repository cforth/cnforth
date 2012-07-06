cforth: cforth.o code_words.o
	gcc $^ -o $@
cforth.o: cforth.c cforth.h code_words.h
	gcc -c cforth.c
code_words.o: code_words.c code_words.h
	gcc -c code_words.c
clean:
	@echo "cleanning project"
	-rm -f cforth *.o
	@echo "clean completed"
