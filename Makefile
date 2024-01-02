build: ./src/3DEngine.c
	gcc -Wall -Wextra ./src/3DEngine.c -lSDL2 -lSDL2_ttf -lm -o 3DEngine

run:
	./3DEngine

clean:
	rm 3DEngine

test:
	gcc -Wall -Wextra ./test.c -lSDL2 -lSDL2_ttf -lm -o test
	./test
	rm test

load: ./src/load_from_object_file.c
	gcc -Wall -Wextra ./src/load_from_object_file.c -lSDL2 -lSDL2_ttf -lm -o load
	./load
	rm load
