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

#############################################################
temp: build_temp run_temp clean_temp  
	@echo ./build/temp done

build_temp: ./src/temp.c
	gcc ./src/temp.c $(CFLAGS) -o ./build/temp

run_temp:
	./build/temp
	@echo

clean_temp:
	@echo
	rm ./build/temp

debug_build_temp: ./src/temp.c
	@gcc ./src/temp.c $(CFLAGS) -g -o ./build/temp
# valgrind -s --leak-check=full ./temp