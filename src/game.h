#ifndef GAME_H
#define GAME_H

#include "input.h"
#include "snake.h"

typedef enum {
    Playing,
    Paused,
    Lost,
    Won
} GameStatus;

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int max_score;
    unsigned int score;
    GameStatus status;
    Position food;
    Snake snake;
} Game;

Game *game_new(unsigned int game_width, unsigned int game_height);

void game_init(Game *game, unsigned int game_width, unsigned int game_height);

void game_free(Game *game);

void game_update(Game *game);

void game_process_input(Game *game, GameInput input);

#endif /* GAME_H */
