#ifndef GAME_H
#define GAME_H

#include "input.h"
#include "snake.h"

typedef enum { Playing, Paused, Lost, Won } GameStatus;

typedef struct {
    unsigned int score;
    unsigned int width;
    unsigned int height;
    unsigned int speed; /* speed of game: in cells travelled per second */
    GameStatus status;
    Position food;
    Snake snake;
} Game;

Game *game_init(unsigned int game_width, unsigned int game_height);

void game_free(Game *game);

void game_update(Game *game);

void game_process_input(Game *game, GameInput input);

#endif /* GAME_H */
