.PHONY:

MAKEFLAGS += -s

CC = g++
CFLAGS = -Wall -Wextra -pedantic -v
INCS = ../../lib/glad.c
LIBS = -lglfw -lGL -lm -ldl -lpthread
XLIBS = -lXrandr -lXi -lX11 -lXxf86vm -lXinerama -lXcursor

main: main.cpp $(INCS)
	$(CC) -o $@.out $^ $(CFLAGS) $(LIBS) $(XLIBS)

clean:
	-rm *.out

.PHONY: clean
