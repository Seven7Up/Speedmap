CC := gcc
CFLAGS := -g -Wall
BASE_NAME := speedmap

all: clean $(BASE_NAME).c
	$(CC) $(CFLAGS) -o $(BASE_NAME).o $(BASE_NAME).c
	
	@echo "------------------------------------------------------------"
	./$(BASE_NAME).o
	@echo "------------------------------------------------------------"


clean: $(BASE_NAME).o