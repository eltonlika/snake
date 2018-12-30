#ifndef RENDERER_H
#define RENDERER_H

#include "game.h"
#include <curses.h>

WINDOW *renderer_init();

void renderer_render(WINDOW *window, Game *game);

unsigned int renderer_get_max_width(WINDOW *window);

unsigned int renderer_get_max_height(WINDOW *window);

void renderer_end(WINDOW *window);

#endif /* RENDERER_H */
