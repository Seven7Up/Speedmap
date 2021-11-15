CC := gcc
CFLAGS := -g -Wall

SRC_FILE := speedmap.c
BIN_FILE := speedmap.dev

SHARED_SRC := $(shell ls | grep '[a-zA-Z]*\.c' | grep -v $(SRC_FILE) | xargs)
SHARED_LIBS := -lpthread

all: clean build

build:
	$(CC) $(CFLAGS) -o $(BIN_FILE) $(SRC_FILE) $(SHARED_SRC) $(SHARED_LIBS)
	
# @echo "----------------------------------------------------------------------------"
# @./$(BIN_FILE)
# @echo "----------------------------------------------------------------------------"

clean:
	rm -f $(BIN_FILE)