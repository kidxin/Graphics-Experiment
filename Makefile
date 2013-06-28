# insert the name of your source file here (omit the .c)
TARGET = graph
ESSENTIAL = include/basic.o include/polygon.o include/circle.o
LIBS = -lglut -lGLU -lGL  -lXext -lX11 -lm

CC = gcc

default: $(TARGET)

all: default

.c.o:
	$(CC) -c $<

$(TARGET): $(TARGET).o $(ESSENTIAL)
	$(CC) $(TARGET).c $(ESSENTIAL) -Wall $(LIBS) -o $(TARGET)

include/basic.o: include/basic.c include/basic.h
	gcc -c include/basic.c -o include/basic.o

include/polygon.o: include/polygon.c include/polygon.h include/basic.h
	gcc -c include/polygon.c -o include/polygon.o

include/circle.o: include/circle.c include/circle.h include/basic.h
	gcc -c include/circle.c -o include/circle.o
	
clean:
	-rm -f *.o $(TARGET)
	-rm -f include/*.o

clear:
	-rm -f *.o 
	-rm -f include/*.o
