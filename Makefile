all:
	gcc -Iinclude src/* -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lglu32 -lopengl32 -lfreeglut -lm -o wolf.exe

linux:
	gcc -Iinclude src/* -lSDL2 -lSDL2_image -lSDL2_mixer -lGL -lGLU -lfreeglut -lm -o wolf