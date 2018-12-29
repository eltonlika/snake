#ifndef GAME_H
#define GAME_H

#include "snake.h"

typedef enum { Playing, Paused, Lost, Won } GameStatus;

typedef enum {
    NoInput = 0,
    KeySpeedUp,
    KeyQuit,
    KeyPause,
    KeyUp,
    KeyRight,
    KeyDown,
    KeyLeft
} GameInput;

typedef struct {
    uint score;
    uint width;
    uint height;
    uint speed; /* speed of game: in cells travelled per second */
    GameStatus status;
    Position food;
    Snake snake;
} Game;

Game *game_init(uint game_width, uint game_height);

void game_free(Game *game);

void game_update(Game *game);

void game_process_input(Game *game, GameInput input);

#endif /* GAME_H */
