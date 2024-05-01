CC = clang
FLAGS = -std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always -O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls

ARGS = 4 4 4 4 4 4 4 4 4 4 4 4 0 0

run : main
	$(CC) $(FLAGS) -o main main.c
	./main $(ARGS)