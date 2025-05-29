all:
	gcc -Iinclude src/* -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lglu32 -lopengl32 -lfreeglut -lm -o wolf.exe

linux:
	gcc -Iinclude src/* -lSDL2 -lSDL2_image -lGL -lGLU -lfreeglut -lm -o wolf
