MAKEFLAGS+=--silent

comp: main.cpp ../lib/glad.c
	g++ main.cpp ../lib/glad.c -lglfw -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor -lassimp 

debug: main.cpp ../lib/glad.c
	g++ main.cpp ../lib/glad.c -lglfw -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor -lassimp -Wall -pedantic --verbose
