# insert the name of your source file here (omit the .c)
TARGET = graph
ESSENTIAL = include/basic.o include/polygon.o include/circle.o include/clipping.o include/shadow.o
BASIC = include/basic
CIRCLE = include/circle
POLY = include/polygon
CLIP = include/clipping
SHADOW = include/shadow
LIBS = -lglut -lGLU -lGL  -lXext -lX11 -lm
CC = gcc

default: $(TARGET)

all: default

.c.o:
	$(CC) -c $<

$(TARGET): $(TARGET).o $(ESSENTIAL)
	$(CC) $(TARGET).c $(ESSENTIAL) -Wall $(LIBS) -o $(TARGET)

$(BASIC).o: $(BASIC).c $(BASIC).h
	$(CC) -c $(BASIC).c -o $(BASIC).o

$(POLY).o: $(POLY).c $(POLY).h $(BASIC).h
	$(CC) -c $(POLY).c -o $(POLY).o

$(CIRCLE).o: $(CIRCLE).c $(CIRCLE).h $(BASIC).h
	$(CC) -c $(CIRCLE).c -o $(CIRCLE).o

$(CLIP).o: $(CLIP).c $(POLY).h $(CLIP).h
	$(CC) -c $(CLIP).c -o $(CLIP).o

$(SHADOW).o: $(SHADOW).c $(POLY).h $(SHADOW).h
	$(CC) -c $(SHADOW).c -o $(SHADOW).o

clean:
	-rm -f *.o $(TARGET)
	-rm -f include/*.o

clear:
	-rm -f *.o *~
	-rm -f include/*.o include/*~
