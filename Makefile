src = $(wildcard src/*.c)
headers = $(wildcard src/*.h)

all: snake.exe

snake.exe: $(src) $(headers) Makefile
	gcc -Wall -Wextra -g -std=c89 -pedantic-errors -o $@ $(src) -lncurses

clean:
	rm -r snake.exe*
