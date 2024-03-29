#ifndef RENDERER_H
#define RENDERER_H

#include "game.h"
#include <curses.h>

typedef struct
{
    unsigned int width;
    unsigned int height;
    Bool color;
    WINDOW *window;
} Renderer;

Renderer *renderer_init();

void renderer_free(Renderer *renderer);

void renderer_render(const Renderer *renderer, const Game *game);

void renderer_end(Renderer *renderer);

#endif /* RENDERER_H */
