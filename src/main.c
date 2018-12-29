#include "game.h"
#include <curses.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static WINDOW *window;
static __sighandler_t old_resize_handler;

void new_resize_handler(int sig) {
    int height, width;

    if (old_resize_handler != NULL) {
        old_resize_handler(sig);
    }

    getmaxyx(window, height, width);
    mvprintw(0, 0, "RESIZED to %d x %d\r", width, height);
    wrefresh(window);
}

GameInput read_input() {
    const int c = wgetch(window);

    switch (c) {
    case ERR:
        return NoInput;
    case KEY_UP:
        return KeyUp;
    case KEY_RIGHT:
        return KeyRight;
    case KEY_DOWN:
        return KeyDown;
    case KEY_LEFT:
        return KeyLeft;
    case 's':
        return KeySpeedUp;
    case 'S':
        return KeySpeedUp;
    case 'p':
        return KeyPause;
    case 'P':
        return KeyPause;
    case 'q':
        return KeyQuit;
    case 'Q':
        return KeyQuit;
    }

    return NoInput;
}

void render(Game *game) {
    const Snake *snake = &game->snake;
    const Position *cells = snake->cells;
    const uint length = snake->length;
    uint idx;
    Position food_pos, cell_pos;

    werase(window);

    for (idx = 0; idx < length; idx++) {
        cell_pos = cells[idx];
        mvwaddch(window, cell_pos.y, cell_pos.x, '#');
    }

    food_pos = game->food;
    mvwaddch(window, food_pos.y, food_pos.x, 'o');

    wrefresh(window);
}

int main() {
    Game *game;
    GameInput input;
    uint sleep_time_us;
    int game_height, game_width;

    window = initscr();
    noecho();
    curs_set(0); /* set cursor invisible */
    cbreak();
    nodelay(window, TRUE);
    keypad(window, TRUE);
    old_resize_handler = signal(SIGWINCH, new_resize_handler);

    getmaxyx(window, game_height, game_width);
    game = game_init((uint)game_width, (uint)game_height);

    /* render first frame of game state */
    render(game);

    while (game->status != Won && game->status != Lost) {
        /* calculate render timeout */
        sleep_time_us = (uint)(1000000 / game->speed);

        /* process all buffered inputs */
        input = read_input();
        while (input != NoInput) {
            if (input == KeyQuit) {
                goto quit;
            }
            if (input == KeySpeedUp) {
                sleep_time_us = sleep_time_us / 40;
            }
            game_process_input(game, input);
            input = read_input();
        }

        /* update game */
        game_update(game);

        /* render game */
        render(game);

        usleep(sleep_time_us);
    };

quit:
    endwin();

    /* print game finish status */
    switch (game->status) {
    case Won:
        printf("You WON!!!\n");
        break;
    case Lost:
        printf("You lost.\n");
        break;
    default:
        break;
    }

    game_free(game);

    return 0;
}
