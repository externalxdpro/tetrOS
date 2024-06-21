// Includes
#include "game.h"
#include "font.h"
#include "keyboard.h"
#include "screen.h"
#include "string.h"
#include "system.h"

// Set the logo
#define LOGO_HEIGHT 5
static const char *LOGO[LOGO_HEIGHT] = {
    "AAA BBB CCC DD  EEE FFF", //
    " A  B    C  D D  E  F  ", //
    " A  BBB  C  DD   E  FFF", //
    " A  B    C  D D  E    F", //
    " A  BBB  C  D D EEE FFF", //
};

// Initialize sprite array
uint8_t TILE_SPRITES[NUM_TILES][TILE_SIZE * TILE_SIZE] = {0};

// from listfist.com/list-of-tetris-levels-by-speed-nes-ntsc-vs-pal
const uint8_t FRAMES_PER_STEP[NUM_LEVELS] = {
    48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4,
    4,  3,  3,  3,  2,  2,  2,  2,  2, 2, 2, 2, 2, 1};

// Set line multipliers
const uint32_t LINE_MULTIPLIERS[4] = {40, 100, 300, 1200};

// Initialize state
struct State state;

// A function that does some actions when the current piece is done
// It flashes the current piece and checks how many lines are full and have to
// be cleared
void done() {
    TTM_FOREACH(x, y, xx, yy, state.curr.x, state.curr.y) {
        if (IN_BOARD(xx, yy) &&
            TTM_BLOCK(state.curr.ttm->rotations[state.curr.r], x, y)) {
            state.board[yy][xx] |= TILE_FLAG_FLASH;
        }
    }

    uint32_t lines = 0;

    for (size_t y = 0; y < BOARD_HEIGHT; y++) {
        bool line = true;

        for (size_t x = 0; x < BOARD_WIDTH; x++) {
            if ((state.board[y][x] & TILE_MASK) == NONE) {
                line = false;
                break;
            }
        }

        if (line) {
            lines++;

            for (size_t x = 0; x < BOARD_WIDTH; x++) {
                state.board[y][x] |= TILE_FLAG_FLASH | TILE_FLAG_DESTROY;
            }

            state.destroy = true;
        }
    }

    if (lines > 0) {
        state.lines += lines;
        state.score += LINE_MULTIPLIERS[lines - 1] * (state.level + 1);

        if (state.level != NUM_LEVELS - 1) {
            state.lines_left -= lines;
            if (state.lines_left <= 0) {
                state.level++;
                state.lines_left = 10;
            }
        }
    }

    state.curr.done = true;
    state.held      = false;
}

// A function that trys to modify a piece (move/rotate)
bool try_modify(const struct Tetromino *ttm, uint16_t tc, int32_t xc,
                int32_t yc, uint16_t tn, int32_t xn, int32_t yn) {
    uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];
    memcpy(&board, &state.board, sizeof(board));

    if (tc != 0) {
        TTM_FOREACH(x, y, xx, yy, xc, yc) {
            if (IN_BOARD(xx, yy) && TTM_BLOCK(tc, x, y)) {
                state.board[yy][xx] = NONE;
            }
        }
    }

    TTM_FOREACH(x, y, xx, yy, xn, yn) {
        if (yy < 0) {
            if (TTM_BLOCK(tn, x, y) && (xx < 0 || xx >= BOARD_WIDTH)) {
                goto fail;
            }

            continue;
        } else if (!TTM_BLOCK(tn, x, y)) {
            continue;
        } else if (!IN_BOARD(xx, yy) || state.board[yy][xx] != NONE || xx < 0 ||
                   xx > (BOARD_WIDTH - 1)) {
            goto fail;
        }

        state.board[yy][xx] = ttm->colour;
    }

    return true;

fail:
    memcpy(&state.board, &board, sizeof(board));
    return false;
}

// A function that spawns a new tetromino
// If a tetromino is put in as an argument, that piece will spawn
// Otherwise, a random piece will be spawned
bool spawn(const struct Tetromino *ttm) {
    if (ttm == NULL) {
        if (state.next == NULL) {
            state.next = &TETROMINOS[rand() % NUM_TETROMINOS];
        }

        state.curr.ttm = state.next;
        state.curr.r   = 0;
        state.curr.x   = (BOARD_WIDTH / 2) - 2;
        state.curr.y   = -TTM_OFFSET_Y(state.curr.ttm->rotations[state.curr.r]);
        state.curr.done = false;

        if (!try_modify(state.curr.ttm, 0, 0, 0,
                        state.curr.ttm->rotations[state.curr.r], state.curr.x,
                        state.curr.y)) {
            return false;
        }

        state.next = &TETROMINOS[rand() % NUM_TETROMINOS];
        return true;
    }

    state.curr.ttm  = ttm;
    state.curr.r    = 0;
    state.curr.x    = (BOARD_WIDTH / 2) - 2;
    state.curr.y    = -TTM_OFFSET_Y(state.curr.ttm->rotations[state.curr.r]);
    state.curr.done = false;

    if (!try_modify(state.curr.ttm, 0, 0, 0,
                    state.curr.ttm->rotations[state.curr.r], state.curr.x,
                    state.curr.y)) {
        return false;
    }

    return true;
}

// A function that moves the current piece a specific number of cells in the x
// and y axis
// Returns true if succeeded
bool move(int32_t dx, int32_t dy) {
    if (try_modify(state.curr.ttm, state.curr.ttm->rotations[state.curr.r],
                   state.curr.x, state.curr.y,
                   state.curr.ttm->rotations[state.curr.r], state.curr.x + dx,
                   state.curr.y + dy)) {
        state.curr.x += dx;
        state.curr.y += dy;
        return true;
    }

    return false;
}

// A function that rotates the current piece (to the right if true is passed in)
// Returns true if succeeded
bool rotate(bool right) {
    uint8_t r = (state.curr.r + (right ? 1 : -1) + 4) % 4;

    if (try_modify(state.curr.ttm, state.curr.ttm->rotations[state.curr.r],
                   state.curr.x, state.curr.y, state.curr.ttm->rotations[r],
                   state.curr.x, state.curr.y)) {
        state.curr.r = r;
        return true;
    }

    return false;
}

// A function that generates the sprites for each tetromino and saves it into
// the array at the top of the file
void generate_sprites() {
    for (enum Tile t = 0; t < NUM_TILES; t++) {
        if (t == NONE) {
            continue;
        }

        uint8_t  colour = TILE_COLOURS[t];
        uint8_t *pixels = TILE_SPRITES[t];

        for (size_t y = 0; y < TILE_SIZE; y++) {
            for (size_t x = 0; x < TILE_SIZE; x++) {
                uint8_t c = colour;

                if (y == 0 || x == 0) {
                    c = COLOUR_ADD(colour, 1);
                } else if (y == TILE_SIZE - 1 || x == TILE_SIZE - 1) {
                    c = COLOUR_ADD(colour, -1);
                }

                pixels[y * TILE_SIZE + x] = c;
            }
        }
    }
}

// A function that renders a specific tile using a colour, and a x and y point
// on the board
void render_tile(enum Tile tile, size_t x, size_t y) {
    uint8_t *pixels = TILE_SPRITES[tile];
    for (size_t j = 0; j < TILE_SIZE; j++) {
        memcpy(&screen_offset(x, y + j), pixels + (j * TILE_SIZE), TILE_SIZE);
    }
}

// A function that renders the border around the board
void render_border() {
    for (size_t y = 0; y < (SCREEN_HEIGHT / TILE_SIZE); y++) {
        size_t yy = BOARD_Y + (y * TILE_SIZE);

        render_tile(BORDER, BOARD_X - TILE_SIZE, yy);
        render_tile(BORDER, BOARD_X + (BOARD_WIDTH * TILE_SIZE), yy);
    }
}

// A function that renders the board
// Fills the background with black and the tiles with their specific colours
void render_board() {
    for (size_t y = 0; y < BOARD_HEIGHT; y++) {
        for (size_t x = 0; x < BOARD_WIDTH; x++) {
            uint8_t   data = state.board[y][x];
            enum Tile tile = data & TILE_MASK;

            size_t xs = BOARD_X + (x * TILE_SIZE);
            size_t ys = BOARD_Y + (y * TILE_SIZE);

            if (data & TILE_FLAG_FLASH) {
                screen_fill(COLOUR(4, 4, 1), xs, ys, TILE_SIZE, TILE_SIZE);
            } else if (tile != NONE) {
                render_tile(tile, xs, ys);
            }
        }
    }
}

// A function that renders the stats and the next and held piece outside the
// border of the screen
void render_ui() {
#define X_OFFSET_RIGHT (BOARD_X + BOARD_WIDTH_PX + (TILE_SIZE * 2))

#define RENDER_STAT(_title, _value, _colour, _x, _y, _w)                       \
    do {                                                                       \
        char buf[32];                                                          \
        itoa((_value), buf, 32);                                               \
        font_str_doubled((_title), (_x), (_y), COLOUR(7, 7, 3));               \
        font_str_doubled(buf, (_x) + (_w)-font_width(buf), (_y) + TILE_SIZE,   \
                         (_colour));                                           \
    } while (0);

    size_t w = font_width("SCORE");
    RENDER_STAT("SCORE", state.score, COLOUR(5, 5, 0), X_OFFSET_RIGHT,
                TILE_SIZE * 1, w);
    RENDER_STAT("LINES", state.lines, COLOUR(5, 3, 0), X_OFFSET_RIGHT,
                TILE_SIZE * 4, w);
    RENDER_STAT("LEVEL", state.level, COLOUR(5, 0, 0), X_OFFSET_RIGHT,
                TILE_SIZE * 7, w);

#define X_OFFSET_LEFT (BOARD_X - (TILE_SIZE * 8))
#define Y_OFFSET_LEFT TILE_SIZE

    font_str_doubled("NEXT", X_OFFSET_LEFT, TILE_SIZE, COLOUR(7, 7, 3));

    for (size_t j = 0; j < TTM_SIZE; j++) {
        for (size_t i = 0; i < TTM_SIZE; i++) {
            uint16_t tiles = state.next->rotations[0];

            if (TTM_BLOCK(tiles, i, j)) {
                render_tile(state.next->colour,
                            X_OFFSET_LEFT +
                                ((i - TTM_OFFSET_X(tiles)) * TILE_SIZE),
                            Y_OFFSET_LEFT + (TILE_SIZE / 2) +
                                ((j - TTM_OFFSET_Y(tiles) + 1) * TILE_SIZE));
            }
        }
    }

    font_str_doubled("HOLD", X_OFFSET_LEFT, TILE_SIZE * 7, COLOUR(7, 7, 3));

    for (size_t j = 0; j < TTM_SIZE; j++) {
        for (size_t i = 0; i < TTM_SIZE; i++) {
            uint16_t tiles = state.hold->rotations[0];

            if (TTM_BLOCK(tiles, i, j)) {
                render_tile(state.hold->colour,
                            X_OFFSET_LEFT +
                                ((i - TTM_OFFSET_X(tiles)) * TILE_SIZE),
                            Y_OFFSET_LEFT * 7 + (TILE_SIZE / 2) +
                                ((j - TTM_OFFSET_Y(tiles) + 1) * TILE_SIZE));
            }
        }
    }
}

// A function that renders the game over screen
// Shows text saying "GAME OVER" and the score
void render_game_over() {
    const size_t w = SCREEN_WIDTH / 3;
    const size_t h = SCREEN_HEIGHT / 3;

    screen_fill(COLOUR(4, 4, 2), (SCREEN_WIDTH - w) / 2,
                (SCREEN_HEIGHT - h) / 2, w, h);
    screen_fill(COLOUR(2, 2, 1), (SCREEN_WIDTH - (w - 8)) / 2,
                (SCREEN_HEIGHT - (h - 8)) / 2, w - 8, h - 8);

    font_str_doubled("GAME OVER", (SCREEN_WIDTH - font_width("GAME OVER")) / 2,
                     (SCREEN_HEIGHT / 2) - TILE_SIZE,
                     (state.frames / 5) % 2 == 0 ? COLOUR(6, 2, 1)
                                                 : COLOUR(7, 4, 2));

    char buf_score[64];
    itoa(state.score, buf_score, 64);

    font_str_doubled("SCORE:", (SCREEN_WIDTH - font_width("SCORE:")) / 2,
                     (SCREEN_HEIGHT / 2), COLOUR(6, 6, 0));
    font_str_doubled(buf_score, (SCREEN_WIDTH - font_width(buf_score)) / 2,
                     (SCREEN_HEIGHT / 2) + TILE_SIZE, COLOUR(7, 7, 3));
}

// A function that holds the current piece if a piece has not been held yet this
// turn
bool hold() {
    if (state.held) {
        return false;
    }

    const struct Tetromino *held = state.hold;
    state.hold                   = state.curr.ttm;

    for (size_t j = 0; j < TTM_SIZE; j++) {
        for (size_t i = 0; i < TTM_SIZE; i++) {
            uint16_t tiles = state.curr.ttm->rotations[state.curr.r];

            if (TTM_BLOCK(tiles, i, j)) {
                state.board[j + state.curr.y][i + state.curr.x] = NONE;
            }
        }
    }

    spawn(held);
    state.held = true;

    return true;
}

// A function that moves the current piece down
// If the piece cannot be moved down for two steps the piece will be placed
void step() {
    bool stopped = !move(0, 1);

    if (stopped && state.stopped) {
        done();
    }

    state.stopped = stopped;
}

// A function that resets the state of the game
void reset(uint32_t level) {
    memset(&state, 0, sizeof(state));
    state.frames_since_step = FRAMES_PER_STEP[level];
    state.level             = level;
    state.lines_left        = state.level * 10 + 10;
    spawn(NULL);
}

// A function that updates the logic of the game every frame
// (stuff like resetting the game when enter is pressed on the game over screen,
// moving the current piece down, checking if the piece is done, checking
// controls, etc...)
void update() {
    if (state.game_over) {
        if (keyboard_char('\n')) {
            reset(0);
        }

        return;
    }

    for (size_t y = 0; y < BOARD_HEIGHT; y++) {
        bool destroy = false;

        for (size_t x = 0; x < BOARD_WIDTH; x++) {
            uint8_t data = state.board[y][x];

            if (data & TILE_FLAG_DESTROY) {
                state.board[y][x] = NONE;
                destroy           = true;
            } else {
                state.board[y][x] &= ~TILE_FLAG_FLASH;
            }
        }

        if (destroy) {
            if (y != 0) {
                memmove(&state.board[1], &state.board[0],
                        sizeof(state.board[0]) * y);
            }

            memset(&state.board[0], NONE, sizeof(state.board[0]));
        }
    }

    if (state.curr.done && !spawn(NULL)) {
        state.game_over = true;
        return;
    }

    if (state.destroy) {
        state.destroy = false;
        return;
    }

    const bool control_states[NUM_CONTROLS] = {
        keyboard_key(KEY_LCTRL),  //
        keyboard_char(KEY_UP),    //
        keyboard_char(KEY_LEFT),  //
        keyboard_char(KEY_RIGHT), //
        keyboard_char(KEY_DOWN),  //
        keyboard_char(' '),       //
        keyboard_key(KEY_LSHIFT)  //
    };

    for (size_t i = 0; i < NUM_CONTROLS; i++) {
        struct Control *c = &state.controls.raw[i];
        c->last           = c->down;
        c->down           = control_states[i];
        c->pressed        = !c->last && c->down;

        if (c->pressed) {
            c->pressed_frames = state.frames;
        }
    }

    if (state.controls.rotate_left.pressed) {
        rotate(false);
    } else if (state.controls.rotate_right.pressed) {
        rotate(true);
    }

    if (state.controls.left.down &&
        (state.frames - state.controls.left.pressed_frames) % 2 == 0) {
        move(-1, 0);
    } else if (state.controls.right.down &&
               (state.frames - state.controls.right.pressed_frames) % 2 == 0) {
        move(1, 0);
    } else if (state.controls.down.down &&
               (state.frames - state.controls.down.pressed_frames) % 2 == 0) {
        if (!move(0, 1)) {
            done();
        }
    } else if (state.controls.drop.pressed) {
        while (move(0, 1)) {}
        done();
    }

    if (state.controls.hold.pressed) {
        hold();
    }

    if (--state.frames_since_step == 0) {
        step();
        state.steps++;
        state.frames_since_step = FRAMES_PER_STEP[state.level];
    }
}

// A function that handles rendering game stuff
void render() {
    screen_clear(COLOUR(0, 0, 0));
    render_border();
    render_board();
    render_ui();

    if (state.game_over) {
        render_game_over();
    }
}

// A function that starts the game if enter is pressed in the menu
void update_menu() {
    if (keyboard_char(KEY_ENTER)) {
        reset(0);
        state.menu = false;
    }
}

// A function that handles rendering menu stuff with animations!!!
void render_menu() {
    screen_clear(COLOUR(0, 0, 0));

    size_t logo_width = strlen(LOGO[0]);
    size_t logo_x     = (SCREEN_WIDTH - (logo_width * TILE_SIZE)) / 2;
    size_t logo_y     = TILE_SIZE * 3;

    for (int32_t x = -1; x < (int32_t)logo_width + 1; x++) {
        render_tile(BORDER, logo_x + (x * TILE_SIZE), logo_y - (TILE_SIZE * 2));
        render_tile(BORDER, logo_x + (x * TILE_SIZE),
                    logo_y + (TILE_SIZE * (1 + LOGO_HEIGHT)));
    }

    for (size_t y = 0; y < LOGO_HEIGHT; y++) {
        for (size_t x = 0; x < logo_width; x++) {
            char c = LOGO[y][x];

            if (c == ' ' || c == '\t' || c == '\n') {
                continue;
            }

            render_tile(c - 'A' + 1, logo_x + (x * TILE_SIZE),
                        logo_y + (y * TILE_SIZE));
        }
    }

    const char *play = "PRESS ENTER TO PLAY";
    font_str_doubled(play, (SCREEN_WIDTH - font_width(play)) / 2,
                     logo_y + ((LOGO_HEIGHT + 6) * TILE_SIZE), COLOUR(6, 6, 2));
}
