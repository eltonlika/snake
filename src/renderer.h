#ifndef RENDERER_H
#define RENDERER_H

#include "game.h"
#include <curses.h>

typedef struct {
    unsigned int width;
    unsigned int height;
    WINDOW *main_window;
} Screen;

Screen *renderer_init();

void renderer_render(Screen *screen, Game *game);

void renderer_end(Screen *screen);

#endif /* RENDERER_H */
