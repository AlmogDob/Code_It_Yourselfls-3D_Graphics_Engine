CFLAGS = -Wall -Wextra -lm -lSDL2 -lSDL2_ttf

main: build run clean
	@echo main done
build: main.c
	gcc ./main.c $(CFLAGS) -o main 

run:
	./main

clean:
	rm main

test:
	gcc -o test $(CFLAGS) ./test.c 
	./test
	rm test

dbug: dbug_build dbug_run clean

dbug_build: main.c
	gcc -o main $(CFLAGS) -g ./main.c 

dbug_run:
	gdb ./main 
