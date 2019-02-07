#ifndef GAME_H
#define GAME_H

#include "input.h"
#include "snake.h"
#include "utils.h"

typedef enum {
    Playing,
    Paused,
    GameOver,
    Quit
} GameStatus;

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int max_snake_length;
    unsigned int score;
    unsigned int milliseconds_per_frame;
    unsigned int input_queue_capacity;
    unsigned int input_queue_count;
    Bool walls;
    GameStatus status;
    Position food;
    Snake snake;
    GameInput *input_queue;
} Game;

Game *game_new(unsigned int game_width, unsigned int game_height, Bool walls);

void game_input(Game *game, GameInput input);

void game_update(Game *game);

void game_free(Game *game);

#endif /* GAME_H */
