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

debug_main: debug_build_main
	gdb ./build/main

debug_build_main: ./src/main.c
	gcc ./src/main.c $(CFLAGS) -ggdb -o ./build/main

profile_main: profile_build_main
	./build/main
	gprof ./build/main
	@echo
	rm gmon.out

profile_build_main: ./src/main.c
	gcc ./src/main.c $(CFLAGS) -p -ggdb -o ./build/main

# valgrind -s --leak-check=full ./main
# cloc --exclude-lang=JSON,make .

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

profile_temp: profile_build_temp
	./build/temp
	@echo
	gprof ./build/temp gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png

profile_build_temp: ./src/temp.c
	gcc ./src/temp.c $(CFLAGS) -p -ggdb -o ./build/temp

# valgrind -s --leak-check=full ./temp
# cloc --exclude-lang=JSON,make .