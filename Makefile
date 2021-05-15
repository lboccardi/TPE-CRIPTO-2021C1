PHONY = all clean

CFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter \
   -pedantic -pedantic-errors -std=c99 -g \
   -fsanitize=address

CC = gcc

EXEC_NAME = main 

OBJ_FILES = src/main.o src/codec.o src/args.o src/decrypt.o src/encrypt.o src/library.o

all: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(EXEC_NAME)
	
clean:
	rm -rf $(OBJ_FILES) $(EXEC_NAME)


