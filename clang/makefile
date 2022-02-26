CC := gcc
CFLAGS := -g -Wall

SRC_FILE := main.c ansi_escapes.c log.c cursor.c
BIN_FILE := main.o

SHARED_LIBS := -lpthread

all: clean build run

build:
	$(CC) $(CFLAGS) -o $(BIN_FILE) $(SRC_FILE) $(SHARED_LIBS)
	
run:
	@echo "----------------------------------------------------------------------------"
	@./$(BIN_FILE); echo "Returned code: $$?"
	@echo "----------------------------------------------------------------------------"

clean:
	rm -f $(BIN_FILE)

gdb: all
	gdb -q ./$(BIN_FILE)
