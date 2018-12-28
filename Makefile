all: snake.exe

snake.exe: snake.c
	gcc -Wall -Wextra -g -std=c89 -pedantic-errors -o snake.exe snake.c

clean:
	rm -r snake.exe*
