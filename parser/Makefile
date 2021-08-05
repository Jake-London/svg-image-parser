CFLAGS = -Wall -g -std=c11
BIN = bin/
INC = include/
SRC = src/
END = ../
REG = ./
XML_PATH = /usr/include/libxml2

# Standard common makefile
parser: $(END)libsvgparse.so

$(END)libsvgparse.so: $(REG)SVGParser.o $(REG)LinkedListAPI.o
	gcc -shared -o $(END)libsvgparse.so $(REG)SVGParser.o $(PAR)LinkedListAPI.o -lxml2 -lm

$(REG)SVGParser.o: $(REG)$(SRC)SVGParser.c $(REG)$(INC)LinkedListAPI.h $(REG)$(INC)SVGParser.h
	gcc -c -fpic $(CFLAGS) -I$(XML_PATH) -I$(REG)$(INC) $(REG)$(SRC)SVGParser.c -o $(REG)SVGParser.o

$(REG)LinkedListAPI.o: $(REG)$(SRC)LinkedListAPI.c $(REG)$(INC)LinkedListAPI.h
	gcc -c -fpic $(CFLAGS) -I$(REG)$(INC) $(REG)$(SRC)LinkedListAPI.c -o $(REG)LinkedListAPI.o

clean:
	rm $(REG)*.o $(END)*.so
