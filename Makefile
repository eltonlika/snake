src = $(wildcard src/*.c : wildcard src/*.h)

all: snake.exe

snake.exe: $(src) Makefile
	gcc -Wall -Wextra -g -std=c89 -pedantic-errors -o $@ $(src)

clean:
	rm -r snake.exe*
