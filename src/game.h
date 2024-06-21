#ifndef GAME_H_
#define GAME_H_

// Includes
#include <stdbool.h>
#include <stdint.h>

#include "screen.h"
#include "util.h"

// Defines
#define FPS 30
#define LEVELS 30

#define TILE_SIZE 10

#define NUM_TILES (BORDER + 1)

// An enum that contains the colour of a tile
enum Tile {
    NONE = 0,
    GREEN,
    ORANGE,
    YELLOW,
    PURPLE,
    PINK,
    BLUE,
    CYAN,
    RED,
    BORDER
};

// Defines
#define TILE_MASK 0x0F
#define TILE_FLAG_FLASH 0x10
#define TILE_FLAG_DESTROY 0x20

static const uint8_t TILE_COLOURS[NUM_TILES] = {
    [NONE]   = COLOUR(7, 0, 3), //
    [GREEN]  = COLOUR(0, 5, 0), //
    [ORANGE] = COLOUR(5, 3, 0), //
    [YELLOW] = COLOUR(5, 5, 0), //
    [PURPLE] = COLOUR(3, 0, 3), //
    [PINK]   = COLOUR(5, 0, 5), //
    [BLUE]   = COLOUR(0, 0, 2), //
    [CYAN]   = COLOUR(0, 3, 3), //
    [RED]    = COLOUR(5, 0, 0), //
    [BORDER] = COLOUR(2, 2, 1), //
};

// Defines
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BOARD_SIZE (BOARD_WIDTH * BOARD_HEIGHT)

#define BOARD_WIDTH_PX (BOARD_WIDTH * TILE_SIZE)
#define BOARD_HEIGHT_PX (BOARD_HEIGHT * TILE_SIZE)
#define BOARD_X ((SCREEN_WIDTH - BOARD_WIDTH_PX) / 2)
#define BOARD_Y (0)

#define IN_BOARD(_x, _y)                                                       \
    ((_x) >= 0 && (_y) >= 0 && (_x) < BOARD_WIDTH && (_y) < BOARD_HEIGHT)

#define TTM_SIZE 4

#define TTM_BLOCK(_t, _i, _j) (((_t) & (1 << (((_j) * 4) + (_i)))) != 0)

#define TTM_OFFSET_X(_t)                                                       \
    MIN(_t & 0x000F ? LOBIT((_t >> 0) & 0xF) : 3,                              \
        MIN(_t & 0x00F0 ? LOBIT((_t >> 4) & 0xF) : 3,                          \
            MIN(_t & 0x0F00 ? LOBIT((_t >> 8) & 0xF) : 3,                      \
                _t & 0xF000 ? LOBIT((_t >> 12) & 0xF) : 3)))
#define TTM_OFFSET_Y(_t) (LOBIT(_t) / 4)

#define TTM_WIDTH(_t)                                                          \
    1 +                                                                        \
        MAX(HIBIT((_t >> 0) & 0xF),                                            \
            MAX(HIBIT((_t >> 4) & 0xF),                                        \
                MAX(HIBIT((_t >> 8) & 0xF), HIBIT((_t >> 12) & 0xF)))) -       \
        TTM_OFFSET_X(_t)
#define TTM_HEIGHT(_t) ((HIBIT(_t) / 4) - (LOBIT(_t) / 4) + 1)

#define TTM_FOREACH(_xname, _yname, _xxname, _yyname, _xbase, _ybase)          \
    for (int32_t _yname = 0, _yyname = (_ybase); _yname < TTM_SIZE;            \
         _yname++, _yyname++)                                                  \
        for (int32_t _xname = 0, _xxname = (_xbase); _xname < TTM_SIZE;        \
             _xname++, _xxname++)

// A struct containing the colour and the rotations of a tetromino
struct Tetromino {
    enum Tile colour;
    uint16_t  rotations[4];
};

// Set the colour and rotations of each tetromino in an array
#define NUM_TETROMINOS 7
static const struct Tetromino TETROMINOS[NUM_TETROMINOS] = {
    {
        // I
        .colour = CYAN,
        .rotations =
            {
                0x00F0,
                0x2222,
                0x0F00,
                0x4444,
            },
    },
    {
        // J
        .colour = BLUE,
        .rotations =
            {
                0x0E20,
                0x44C0,
                0x8E00,
                0x6440,
            },
    },
    {
        // L
        .colour = ORANGE,
        .rotations =
            {
                0x0E80,
                0xC440,
                0x2E00,
                0x4460,
            },
    },
    {
        // O
        .colour = YELLOW,
        .rotations =
            {
                0xCC00,
                0xCC00,
                0xCC00,
                0xCC00,
            },
    },
    {
        // S
        .colour = GREEN,
        .rotations =
            {
                0x6C00,
                0x4620,
                0x06C0,
                0x8C40,
            },
    },
    {
        // Z
        .colour = RED,
        .rotations =
            {
                0xC600,
                0x2640,
                0x0C60,
                0x4C80,
            },
    },
    {
        // T
        .colour = PURPLE,
        .rotations =
            {
                0x0E40,
                0x4C40,
                0x4E00,
                0x4640,
            },
    },
};

#define NUM_LEVELS 30

#define NUM_CONTROLS 7

// A struct that contains the states of a button
struct Control {
    bool     down;
    bool     last;
    bool     pressed;
    uint32_t pressed_frames;
};

// A struct that contains the state of the game currently
struct State {
    uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];

    uint32_t frames, steps, frames_since_step;
    uint32_t score, lines, level;
    int32_t  lines_left;
    bool     menu, pause, stopped, destroy, game_over, held;

    const struct Tetromino *next, *hold;

    struct {
        const struct Tetromino *ttm;
        uint8_t                 r;
        int32_t                 x, y;
        bool                    done;
    } curr;

    union {
        struct {
            struct Control rotate_left;
            struct Control rotate_right;
            struct Control left;
            struct Control right;
            struct Control down;
            struct Control drop;
            struct Control hold;
        };
        struct Control raw[NUM_CONTROLS];
    } controls;
};
extern struct State state;

// Functions
void generate_sprites();

void update();
void render();

void update_menu();
void render_menu();

#endif // GAME_H_
