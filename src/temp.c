#define SETUP
#define UPDATE
#define RENDER
#include "./../include/display.c"
#define MATRIX2D_IMPLEMENTATION
#include "./../include/Matrix2D.h"
#define ALMOG_RENDER_SHAPES_IMPLEMENTATION
#include "./../include/Almog_Render_Shapes.h"

void setup(game_state_t *game_state)
{
    (void)game_state;
}

void update(game_state_t *game_state)
{
    (void)game_state;
}

void render(game_state_t *game_state)
{
    ars_draw_line(game_state->window_pixels_mat, 100, 100, 200, 300, 0xFFFFFF);
}