CC=cc

SOURCE=src
INCLUDE=include
TARGET=target

EXECUTABLE=$(TARGET)/crisp2
TESTS_EXECUTABLE=$(EXECUTABLE)_tests

CFLAGS=-Wall -I $(INCLUDE)

DEPS=$(addprefix $(TARGET)/, parser.o)

build: prepare $(DEPS) $(TARGET)/main.o
	gcc -o $(EXECUTABLE) $(DEPS) $(TARGET)/main.o

run: build
	./$(EXECUTABLE)

tests: prepare $(DEPS) $(TARGET)/tests.o
	gcc -o $(TESTS_EXECUTABLE) $(DEPS) $(TARGET)/tests.o

run_tests: tests
	./$(TESTS_EXECUTABLE)

$(TARGET)/%.o: $(SOURCE)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

prepare:
	mkdir -p target/

clean:
	rm -f $(TARGET)/*

.PHONY: prepare clean
