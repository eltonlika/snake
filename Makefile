src = $(wildcard src/*.c)
headers = $(wildcard src/*.h)

all: snake.exe

snake.exe: $(src) $(headers) Makefile
	gcc -lcurses -Wall -Wextra -g -std=c89 -pedantic-errors -o $@ $(src)

clean:
	rm -r snake.exe*
