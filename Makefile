CC=cc
SOURCE=src
INCLUDE=include
TARGET=target
CFLAGS=-I $(INCLUDE)

default: prepare main.o parser.o
	gcc -o $(TARGET)/crisp2 $(TARGET)/*.o

%.o: $(SOURCE)/%.c
	$(CC) -c -o $(TARGET)/$@ $< $(CFLAGS)

prepare:
	mkdir -p target/

clean:
	rm -f $(TARGET)/*

.PHONY: prepare clean
