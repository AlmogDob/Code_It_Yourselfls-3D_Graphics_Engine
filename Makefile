CFLAGS = -Wall -Wextra -lm -lSDL2 -lSDL2_ttf

main: build run clean
	@echo main done
build: ./src/main.c
	gcc ./src/main.c $(CFLAGS) -o ./src/main 

run:
	./src/main

clean:
	rm ./src/main

test:
	gcc -o test $(CFLAGS) ./test.c 
	./test
	rm test

dbug: dbug_build dbug_run clean

dbug_build: main.c
	gcc -o main $(CFLAGS) -g ./main.c 

dbug_run:
	gdb ./main 
