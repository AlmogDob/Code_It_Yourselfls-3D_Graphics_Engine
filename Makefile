CFLAGS = -Wall -Wextra -lm -lSDL2 -lSDL2_ttf
CCHECKS = -fsanitize=address

main: build_main run_main clean_main  
	@echo ./build/main done

build_main: ./src/main.c
	gcc ./src/main.c $(CFLAGS) -o ./build/main

run_main:
	@echo
	./build/main

clean_main:
	@echo
	rm ./build/main

debug_main: debug_build_main
	gdb ./build/main

debug_build_main: ./src/main.c
	gcc ./src/main.c $(CFLAGS) -ggdb -o ./build/main

profile_main: profile_build_main
	./build/main
	@echo
	gprof ./build/main gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png 
	make clean_main

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
	@echo
	./build/temp

clean_temp:
	@echo
	rm ./build/temp

debug_temp: debug_build_temp
	gdb ./build/temp

debug_build_temp: ./src/temp.c
	@gcc ./src/temp.c $(CFLAGS) -ggdb -o ./build/temp

profile_temp: profile_build_temp
	./build/temp
	@echo
	gprof ./build/temp gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png 
	make clean_temp

profile_build_temp: ./src/temp.c
	gcc ./src/temp.c $(CFLAGS) -p -ggdb -o ./build/temp

# valgrind -s --leak-check=full ./temp
# cloc --exclude-lang=JSON,make .

#############################################################
video_ship: build_Aobj2c
	@echo
	cat ./obj_files/video_ship.obj | ./build/Aobj2c > ./build/video_ship.c
	@echo
	rm ./build/Aobj2c
	@echo ./build/Aobj2c done

axis: build_Aobj2c
	@echo
	cat ./obj_files/axis.obj | ./build/Aobj2c > ./build/axis.c
	@echo
	rm ./build/Aobj2c
	@echo ./build/Aobj2c done

teapot: build_Aobj2c
	@echo
	cat ./obj_files/teapot.obj | ./build/Aobj2c > ./build/teapot.c
	@echo
	rm ./build/Aobj2c
	@echo ./build/Aobj2c done

Aobj2c: build_Aobj2c
	@echo
	@cat input.txt | ./build/Aobj2c 
	@echo
	rm ./build/Aobj2c
	@echo ./build/Aobj2c done

build_Aobj2c: ./src/Aobj2c.c
	gcc ./src/Aobj2c.c $(CFLAGS) -o ./build/Aobj2c

run_Aobj2c:
	./build/Aobj2c

clean_Aobj2c:
	@echo
	rm ./build/Aobj2c

debug_build_Aobj2c: ./src/Aobj2c.c
	@gcc ./src/Aobj2c.c $(CFLAGS) -g -o ./build/Aobj2c

profile_Aobj2c: profile_build_Aobj2c
	./build/Aobj2c
	@echo
	gprof ./build/Aobj2c gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png 
	make clean_Aobj2c

profile_build_Aobj2c: ./src/Aobj2c.c
	gcc ./src/Aobj2c.c $(CFLAGS) -p -ggdb -o ./build/Aobj2c

# valgrind -s --leak-check=full ./Aobj2c
# cloc --exclude-lang=JSON,make .