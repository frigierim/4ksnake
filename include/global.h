#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <mmsystem.h>
#include <malloc.h>


#define TARGET_FPS      60
#define MS_PER_FRAME    (1000/TARGET_FPS)

// screen resolution (probably 4:3)
#define SXRES    1024/2
#define SYRES    768/2

// image resolution (probably 16:9)
#define IXRES    SXRES      //1024/2
#define IYRES    SYRES //576/2

// cell size
#define CELLW   16
#define CELLH   16


// grid size
#define GRIDW   (IXRES/CELLW)
#define GRIDH   (IYRES/CELLH)

#define MAX_SNAKE_LEN GRIDW*GRIDH

#define MIN_ALL        419
#define MIN_ALL_UNDO   416

#define INTRO_DURATION 16

enum
{   DIR_N,
    DIR_E,
    DIR_S,
    DIR_W
};

typedef struct
{
    unsigned char x;
    unsigned char y;
} GRID_POS;

typedef struct
{
    unsigned char B;
    unsigned char G;
    unsigned char R;
    unsigned char A;

} QR_COLOR;

typedef struct
{
    QR_COLOR buffer[SXRES*SYRES];
    GRID_POS snake[MAX_SNAKE_LEN];
    short int snake_head;
    unsigned short int snake_len;
    GRID_POS apple;
    unsigned char tick;
    unsigned char speed;
    unsigned char direction;
    unsigned char started;
} GAME_STRUCT;
