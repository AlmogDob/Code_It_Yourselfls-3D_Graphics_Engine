CFLAGS = -Wall -Wextra -lm -lSDL2 -lSDL2_ttf
CCHECKS = -fsanitize=address
LIST_OBJ_C_FILE = ./build/suzanne.c ./build/f16.c ./build/cruiser.c ./build/bunny.c ./build/axis.c ./build/teapot.c  ./build/simple_shape.c ./build/LEGO_Man.c
LIST_OBJ_FILE = ./obj_files/suzanne/suzanne.obj ./obj_files/f16/f16.obj ./obj_files/cruiser/cruiser.obj ./obj_files/bunny.obj ./obj_files/axis.obj ./obj_files/teapot.obj ./obj_files/simple_shape/simple_shape.obj ./obj_files/lego_man/LEGO_Man.obj
BUILD_OBJ_FILES = gcc ./src/Aobj2c.c $(CFLAGS) -o ./build/Aobj2c; \
	cat ./obj_files/axis.obj | ./build/Aobj2c > ./build/axis.c; \
	cat ./obj_files/bunny.obj | ./build/Aobj2c > ./build/bunny.c; \
	cat ./obj_files/f16/f16.obj | ./build/Aobj2c > ./build/f16.c; \
	cat ./obj_files/suzanne/suzanne.obj | ./build/Aobj2c > ./build/suzanne.c; \
	cat ./obj_files/cruiser/cruiser.obj | ./build/Aobj2c > ./build/cruiser.c; \
	cat ./obj_files/teapot.obj | ./build/Aobj2c > ./build/teapot.c; \
	cat ./obj_files/simple_shape/simple_shape.obj | ./build/Aobj2c > ./build/simple_shape.c; \
	cat ./obj_files/lego_man/LEGO_Man.obj | ./build/Aobj2c > ./build/LEGO_Man.c

main: build_main run_main clean_main  
	@echo ./build/main done

build_main: ./src/main.c $(LIST_OBJ_FILE)
	@echo [Info] building obj files
	@$(BUILD_OBJ_FILES)
	@echo [Info] building temp
	@gcc ./src/main.c $(CFLAGS) -o ./build/main

run_main:
	@echo
	./build/main
	@echo

clean_main:
	@echo [Info] removing all build files
	@rm ./build/Aobj2c ./build/main $(LIST_OBJ_C_FILE)

debug_main: debug_build_main
	gdb ./build/main

debug_build_main: ./src/main.c
	@echo [Info] building obj files
	@$(BUILD_OBJ_FILES)
	@echo [Info] building main
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

profile_build_main: ./src/main.c $(LIST_OBJ_FILE)
	@echo [Info] building obj files
	@$(BUILD_OBJ_FILES)
	@echo [Info] building main
	@gcc ./src/main.c $(CFLAGS) -p -ggdb -o ./build/main

# valgrind -s --leak-check=full ./main
# cloc --exclude-lang=JSON,make .

#############################################################
temp: build_temp run_temp clean_temp  
	@echo ./build/temp done

build_temp: ./src/temp.c $(LIST_OBJ_FILE)
	@echo [Info] building obj files
	@$(BUILD_OBJ_FILES)
	@echo [Info] building temp
	@gcc ./src/temp.c $(CFLAGS) -o ./build/temp

run_temp:
	@echo
	./build/temp
	@echo

clean_temp:
	@echo [Info] removing all build files
	@rm ./build/Aobj2c ./build/temp $(LIST_OBJ_C_FILE) 

debug_temp: debug_build_temp
	gdb ./build/temp

debug_build_temp: ./src/temp.c
	@echo [Info] building obj files
	@$(BUILD_OBJ_FILES)
	@echo [Info] building temp
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

profile_build_temp: ./src/temp.c $(LIST_OBJ_FILE)
	@echo [Info] building obj files
	@$(BUILD_OBJ_FILES)
	@echo [Info] building temp
	@gcc ./src/temp.c $(CFLAGS) -p -ggdb -o ./build/temp

# valgrind -s --leak-check=full ./temp
# cloc --exclude-lang=JSON,make .

#############################################################
build_obj_files: $(LIST_OBJ_FILE)
	@echo [Info] building obj files
	@$(BUILD_OBJ_FILES)
	@rm ./build/Aobj2c

video_ship: build_Aobj2c
	@echo
	cat ./obj_files/video_ship.obj | ./build/Aobj2c > ./build/video_ship.c
	@echo
	rm ./build/Aobj2c
	@echo ./build/Aobj2c done

skull: build_Aobj2c
	@echo
	@./build/Aobj2c ./obj_files/Skull/Skull_v3_L2.obj
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