CFLAGS = -Wall -Wextra -lm -lSDL2 -lSDL2_ttf
CCHECKS = -fsanitize=address

main: build_main run_main clean_main  
	@echo ./build/main done

build_main: ./src/main.c
	gcc ./src/main.c $(CFLAGS) -o ./build/main

run_main:
	./build/main
	@echo

clean_main:
	@echo
	rm ./build/main

debug_build_main: ./src/main.c
	@gcc ./src/main.c $(CFLAGS) -g -o ./build/main
# valgrind -s --leak-check=full ./main

