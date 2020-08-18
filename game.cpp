#include "include/global.h"

const QR_COLOR COLOR_RED = {0, 0, 255, 255};
const QR_COLOR COLOR_WHITE = {255, 255, 255, 255};
const QR_COLOR COLOR_BLACK = {0, 0, 0, 255};

static const unsigned char title[] = {
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x40, 0x00, 
  0x00, 0x0e, 0x40, 0x00, 
  0x00, 0x1e, 0x4c, 0x00, 
  0x00, 0x36, 0x58, 0x00, 
  0x00, 0x66, 0x60, 0x00, 
  0x00, 0x7f, 0x58, 0x00, 
  0x00, 0x06, 0x4c, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x39, 0x08, 0x04, 0x0e, 
  0x45, 0x88, 0xc4, 0x90, 
  0x41, 0x48, 0xc4, 0x90, 
  0x39, 0x29, 0x25, 0x1e, 
  0x05, 0x29, 0xe7, 0x90, 
  0x45, 0x1a, 0x14, 0x90, 
  0x39, 0x0a, 0x14, 0x4e
};

// Poor man's random generator
unsigned int PRNG()
{
    return __rdtsc() * __rdtsc() * __rdtsc();
}

static void render_set_pixel(QR_COLOR *buffer, QR_COLOR c, int x, int y)
{
    buffer[x + y * IXRES] = c;
}

static void render_draw_rect(QR_COLOR *buffer, QR_COLOR color, int x, int y, int w, int h, bool filled)
{
    for(int i=x+w-1; i>=x;--i)
        for(int j=y+h-1; j>=y;--j)
            if (j == y || j == y+h-1 || i == x || i == x+w-1 || filled)
                render_set_pixel(buffer, color, i, j);
}

static void render_draw_snake(GAME_STRUCT *p_game, QR_COLOR col)
{
    short int h = p_game->snake_head;
    for(int len = 0; len < p_game->snake_len;++len)
    {
        render_draw_rect(p_game->buffer, col, 1 + p_game->snake[h].x *CELLW, 1 + p_game->snake[h].y*CELLH, CELLW-2, CELLH-2, len==0);
        ++h;
        h%=MAX_SNAKE_LEN;
    }
    
    render_draw_rect(p_game->buffer, COLOR_BLACK, 1 + p_game->snake[h].x *CELLW, 1 + p_game->snake[h].y*CELLH, CELLW-2, CELLH-2, TRUE);
}

static void render_apple(GAME_STRUCT *p_game)
{
    render_draw_rect(p_game->buffer, COLOR_RED, 1 + p_game->apple.x*CELLW, 1 +  p_game->apple.y*CELLH, CELLW-2, CELLH-2, TRUE);
}

static void render_draw_title(QR_COLOR *buffer)
{
    int x = 0, y=0;
    for(unsigned char c = 0; c < sizeof(title);++c)
    {
        if (c % 4 == 0)
        {
            x = 0;
            y+=CELLH;        
        }

        unsigned char val = title[c];
        for (unsigned char i=0; i<8;++i)
        {
            if (val & 0x80)
            {
                render_draw_rect(buffer, COLOR_WHITE, x, y, CELLW, CELLH, TRUE);
            }
            val <<= 1;
            x+= CELLW;
        } 

    }
}

static void init_snake(GAME_STRUCT *p_game)
{
    p_game->snake[0].x = GRIDW/2;
    p_game->snake[0].y = GRIDH/2;
    p_game->direction = DIR_E;
    p_game->snake_head = 0;
    p_game->snake_len = 1;
}

static BOOL check_snake_at(GAME_STRUCT *p_game, unsigned char pos_x, unsigned char pos_y)
{
    short int h = p_game->snake_head;
    for(int len = 0; len < p_game->snake_len;++len)
    {
        if (p_game->snake[h].x == pos_x && p_game->snake[h].y == pos_y)
        {
            return TRUE;
        }
        ++h;
        h %=MAX_SNAKE_LEN;
    }
    return FALSE;
}

static void spawn_new_apple(GAME_STRUCT *p_game)
{
    // Spawn a new apple        
    unsigned char app_x, app_y;
    do
    {
        app_x = PRNG() % GRIDW;
        app_y = PRNG() % GRIDH;
    } while(check_snake_at(p_game, app_x, app_y) == TRUE);
    p_game->apple.x = app_x;
    p_game->apple.y = app_y;
}

static BOOL advance_snake(GAME_STRUCT *p_game)
{
    unsigned char x = p_game->snake[p_game->snake_head].x, y = p_game->snake[p_game->snake_head].y;

    switch(p_game->direction)
    {
        case DIR_N:
            if (y>0)
                --y;
            else
                y = GRIDH-1;
            break;

        case DIR_E:
            if (x<GRIDW-1)
                ++x;
            else
                x = 0;
            break;

        case DIR_S:
            if (y<GRIDH-1)
                ++y;
            else
                y = 0;
            break;

        case DIR_W:
            if (x>0)
                --x;
            else
                x = GRIDW-1;
            break;

        default:
            break;
    }

    // Crashed with itself?
    if (check_snake_at(p_game, x,y))
        return FALSE;

    // move snake
    if(--p_game->snake_head < 0)
        p_game->snake_head += MAX_SNAKE_LEN;
    p_game->snake[p_game->snake_head].x = x;
    p_game->snake[p_game->snake_head].y = y;

    // Collected apple
    if (x == p_game->apple.x && y == p_game->apple.y)
    {
        ++p_game->snake_len;
        
        if (p_game->speed > 1)
            --p_game->speed;

        spawn_new_apple(p_game);
    } 
    
    return TRUE;
}

void init_game(GAME_STRUCT *p_game)
{
    p_game->started = FALSE;
    p_game->speed = TARGET_FPS >> 3;
    init_snake(p_game);
    spawn_new_apple(p_game);
}

// Function called every MS_PER_FRAME millis
void process_frame(GAME_STRUCT *p_game, bool up, bool down, bool left, bool right)
{
    static unsigned short start_timer = 0;

    p_game->tick++;

    if (p_game->tick > p_game->speed)
    {
        // TICK!
        p_game->tick = 0;
        
        // Decide next direction
        if (down ^ up)
            p_game->direction = down && p_game->direction != DIR_N ? DIR_S : p_game->direction != DIR_S ? DIR_N : p_game->direction;
        else if (right ^ left)
            p_game->direction = right && p_game->direction != DIR_W ? DIR_E : p_game->direction != DIR_E ? DIR_W : p_game->direction;

        // Hit some snake?
        if (advance_snake(p_game) == FALSE)
        {
            // GAME OVER
            init_game(p_game);
            render_draw_rect(p_game->buffer, COLOR_BLACK,     0, 0, IXRES, IYRES, true);
        }

    }

    render_draw_rect(p_game->buffer, COLOR_WHITE,     0, 0, IXRES, IYRES, false);
    
    if (p_game->started)
    {
        render_draw_snake(p_game, COLOR_WHITE);
        render_apple(p_game);
    }
    else
    {
        render_draw_title(p_game->buffer);
        if (p_game->tick == 0)
        {
            ++start_timer;
            if (start_timer > INTRO_DURATION)
            {
                start_timer = 0;
                p_game->started = TRUE;
                render_draw_rect(p_game->buffer, COLOR_BLACK,     0, 0, IXRES, IYRES, true);
            }
        }
    }
}
