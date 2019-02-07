#ifndef RENDERER_H
#define RENDERER_H

#include "game.h"
#include <curses.h>

typedef struct {
    unsigned int width;
    unsigned int height;
    WINDOW *window;
} Renderer;

Renderer *renderer_init();

void renderer_free(Renderer *renderer);

void renderer_render(Renderer *renderer, Game *game);

void renderer_end(Renderer *renderer);

#endif /* RENDERER_H */
