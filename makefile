cforth: cforth.o
	gcc $< -o $@
cforth.o: cforth.c code_words.h colon_words.h
	gcc -c cforth.c
clean:
	@echo "cleanning project"
	-rm -f cforth *.o
	@echo "clean completed"
