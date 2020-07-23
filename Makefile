CC=cc
CFLAGS=
SOURCE=src
TARGET=target

default: prepare main.o
	gcc -o $(TARGET)/crisp2 $(TARGET)/*.o

%.o: $(SOURCE)/%.c
	$(CC) -c -o $(TARGET)/$@ $< $(CFLAGS)

prepare:
	mkdir -p target/

clean:
	rm -f $(TARGET)/*

.PHONY: prepare clean
