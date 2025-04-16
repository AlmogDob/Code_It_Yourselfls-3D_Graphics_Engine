#define SETUP
#define UPDATE
#define RENDER
#include "./../include/display.c"
#define MATRIX2D_IMPLEMENTATION
#include "./../include/Matrix2D.h"
#define ALMOG_RENDER_SHAPES_IMPLEMENTATION
#include "./../include/Almog_Render_Shapes.h"

void ars_draw_line(Mat2D screen_mat, int x1, int y1, int x2, int y2, uint32_t color)
{
    if ((x1 < (int)screen_mat.cols && y1 < (int)screen_mat.rows && x2 < (int)screen_mat.cols && y2 < (int)screen_mat.rows) && (x1 >= 0 && y1 >= 0 && x2 >= 0 && y2 >= 0)) {
        MAT2D_AT(screen_mat, x1, y1) = color;
        MAT2D_AT(screen_mat, x2, y2) = color;
    }
}

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
    ars_draw_line(game_state->window_pixels_mat, 100, 100, 200, 200, 0xFFFFFF);
}