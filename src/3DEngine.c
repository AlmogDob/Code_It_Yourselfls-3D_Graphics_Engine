/* This code is heavily inspired by the 3D engine of 'OneLoneCoder' in C++
you can find the source code in:
https://github.com/OneLoneCoder/Javidx9/tree/master/ConsoleGameEngine/BiggerProjects/Engine3D .
featured in this video of his:
https://youtu.be/ih20l3pJoeU?si=CzQ8rjk5ZEOlqEHN .*/


/* Does not work*/



#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define MATRIX_IMPLEMENTATION
#include "Matrix.h"

#define VEC3_IMPLEMENTATION
#include "Vec3.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1000
#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define PRINT_TRIANGLE(t, padding) print_triangle(t, padding, #t)

#define dprintINT(expr) printf(#expr " = %d\n", expr)
#define dprintF(expr) printf(#expr " = %g\n", expr)

#define PI 3.14159265359

#define NUM_OF_TRIANGLES 100

typedef struct {
    Vec3 p[3];
} triangle;

typedef struct {
    triangle tris[NUM_OF_TRIANGLES];
} mesh;

int initialize_window(void);
void setup(void);
void process_input(void);
void update(void);
void render(void);
void destroy_window(void);
void fix_framerate(void);
void print_triangle(triangle t, int padding, char *name);
void mat4x4_dot_vec3(Vec3 *out, Vec3 *in, Mat m);
void SDL_DrawTriangle(SDL_Renderer *current_renderer, triangle t, SDL_Color color);

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;
SDL_Surface *text_surface = NULL;
SDL_Texture *text_texture = NULL;
SDL_Rect fps_place;
SDL_Color white_color;
int game_is_running = 0;
float delta_time;
float fps = 0;
mesh cube_mesh; 
Mat proj_mat, rotZ_mat, rotX_mat;

int current_num_of_triangles = 0;
float theta = 0;


int main()
{
    srand(time(0));
    game_is_running = !initialize_window();

    setup();

    while (game_is_running) {
        process_input();
        update();
        render();        
    }

    destroy_window();
    return 0;
}

int initialize_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return -1;
    }

    window = SDL_CreateWindow(NULL,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              0
                              );
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Erorr creating SDL renderer.\n");
        return -1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "Erorr initailizin SDL_ttf.\n");
        return -1;
    }

    font = TTF_OpenFont("./font/Gabriely Black.ttf",32);
    if (!font) {
        fprintf(stderr, "Error loading font.\n");
        return -1;
    }
    
    return 0;
}

void setup(void)
{    
    white_color.a = 255;
    white_color.b = 255;
    white_color.g = 255;
    white_color.r = 255;

    fps_place.x = 10;
    fps_place.y = 10;
    fps_place.w = 110;
    fps_place.h = 25;

    /*SOUTH*/
    cube_mesh.tris[0].p[0] = Vec3_new(0,0,0);
    cube_mesh.tris[0].p[1] = Vec3_new(0,1,0);
    cube_mesh.tris[0].p[2] = Vec3_new(1,1,0);

    cube_mesh.tris[1].p[0] = Vec3_new(0,0,0);
    cube_mesh.tris[1].p[1] = Vec3_new(1,1,0);
    cube_mesh.tris[1].p[2] = Vec3_new(1,0,0);

    /*EAST*/
    cube_mesh.tris[2].p[0] = Vec3_new(1,0,0);
    cube_mesh.tris[2].p[1] = Vec3_new(1,1,0);
    cube_mesh.tris[2].p[2] = Vec3_new(1,1,1);

    cube_mesh.tris[3].p[0] = Vec3_new(1,0,0);
    cube_mesh.tris[3].p[1] = Vec3_new(1,1,1);
    cube_mesh.tris[3].p[2] = Vec3_new(1,0,1);

    /*NORTH*/
    cube_mesh.tris[4].p[0] = Vec3_new(1,0,1);
    cube_mesh.tris[4].p[1] = Vec3_new(1,1,1);
    cube_mesh.tris[4].p[2] = Vec3_new(0,1,1);

    cube_mesh.tris[5].p[0] = Vec3_new(1,0,1);
    cube_mesh.tris[5].p[1] = Vec3_new(0,1,1);
    cube_mesh.tris[5].p[2] = Vec3_new(0,0,1);

    /*WEST*/
    cube_mesh.tris[6].p[0] = Vec3_new(0,0,1);
    cube_mesh.tris[6].p[1] = Vec3_new(0,1,1);
    cube_mesh.tris[6].p[2] = Vec3_new(0,1,0);

    cube_mesh.tris[7].p[0] = Vec3_new(0,0,1);
    cube_mesh.tris[7].p[1] = Vec3_new(0,1,0);
    cube_mesh.tris[7].p[2] = Vec3_new(0,0,0);

    /*TOP*/
    cube_mesh.tris[8].p[0] = Vec3_new(0,1,0);
    cube_mesh.tris[8].p[1] = Vec3_new(0,1,1);
    cube_mesh.tris[8].p[2] = Vec3_new(1,1,1);

    cube_mesh.tris[9].p[0] = Vec3_new(0,1,0);
    cube_mesh.tris[9].p[1] = Vec3_new(1,1,1);
    cube_mesh.tris[9].p[2] = Vec3_new(1,1,0);

    /*BOTTOM*/
    cube_mesh.tris[10].p[0] = Vec3_new(1,0,1);
    cube_mesh.tris[10].p[1] = Vec3_new(0,0,1);
    cube_mesh.tris[10].p[2] = Vec3_new(0,0,0);

    cube_mesh.tris[11].p[0] = Vec3_new(1,0,1);
    cube_mesh.tris[11].p[1] = Vec3_new(0,0,0);
    cube_mesh.tris[11].p[2] = Vec3_new(1,0,0);

    current_num_of_triangles = 12;

    // for (int i = 0; i < current_num_of_triangles; i++) {
    //     printf("%d .", i+1); PRINT_TRIANGLE(cube_mesh.tris[i], 4);
    // }

    float Near = 0.1f;
    float Far = 1000.0f;
    float FoV = 90.0f;
    float Aspect_Ratio = (float)WINDOW_HEIGHT / (float)WINDOW_WIDTH;
    float FoV_Rad = 1.0f / tanf((FoV * 0.5f *PI)/ 180.0f);

    proj_mat = mat_alloc(4,4);
    rotZ_mat = mat_alloc(4,4);
    rotX_mat = mat_alloc(4,4);
    mat_fill(proj_mat, 0.0f);
    mat_fill(rotZ_mat, 0.0f);
    mat_fill(rotX_mat, 0.0f);

    /* Projection Matrix */
    MAT_AT(proj_mat, 0, 0) = Aspect_Ratio * FoV_Rad;
    MAT_AT(proj_mat, 1, 1) = FoV_Rad;
    MAT_AT(proj_mat, 2, 2) = Far / (Far - Near);
    MAT_AT(proj_mat, 3, 2) = (-Far * Near) / (Far - Near);
    MAT_AT(proj_mat, 2, 3) = 1.0f;

    
}

void process_input(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_is_running = 0;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    game_is_running = 0;
                }
                break;
        }
    }
}

void update(void)
{
    fix_framerate();

    fps = 1.0f / delta_time;

    char fps_count[100];
    sprintf(fps_count, "FPS = %8.4g", fps);
    text_surface = TTF_RenderText_Solid(font, fps_count,white_color);

    text_texture = SDL_CreateTextureFromSurface(renderer,text_surface);
    SDL_FreeSurface(text_surface);

    /* ---------------------------------------------- */

    theta += 1.0f * delta_time;

    /* Rotation Z Matrix */
    MAT_AT(rotZ_mat, 0, 0) = cosf(theta);
    MAT_AT(rotZ_mat, 0, 1) = sinf(theta);
    MAT_AT(rotZ_mat, 1, 0) = -sinf(theta);
    MAT_AT(rotZ_mat, 1, 1) = cosf(theta);
    MAT_AT(rotZ_mat, 2, 2) = 1.0f;
    MAT_AT(rotZ_mat, 3, 3) = 1.0f;

    /* Rotation X Matrix */
    MAT_AT(rotX_mat, 0, 0) = 1;
    MAT_AT(rotX_mat, 1, 1) = cosf(theta*0.5);
    MAT_AT(rotX_mat, 1, 2) = sinf(theta*0.5);
    MAT_AT(rotX_mat, 2, 1) = -sinf(theta*0.5);
    MAT_AT(rotX_mat, 2, 2) = cosf(theta*0.5);
    MAT_AT(rotX_mat, 3, 3) = 1.0f;

}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    /* Draw Triangles */
    triangle projected_tri, translated_tri, rotatedZ_tri, rotatedZX_tri, tri;

    for (int i = 0; i < current_num_of_triangles; i++) {
        
        tri = cube_mesh.tris[i];
        
        mat4x4_dot_vec3(&rotatedZ_tri.p[0], &tri.p[0], rotZ_mat);
        mat4x4_dot_vec3(&rotatedZ_tri.p[1], &tri.p[1], rotZ_mat);
        mat4x4_dot_vec3(&rotatedZ_tri.p[2], &tri.p[2], rotZ_mat);

        mat4x4_dot_vec3(&rotatedZX_tri.p[0], &rotatedZ_tri.p[0], rotX_mat);
        mat4x4_dot_vec3(&rotatedZX_tri.p[1], &rotatedZ_tri.p[1], rotX_mat);
        mat4x4_dot_vec3(&rotatedZX_tri.p[2], &rotatedZ_tri.p[2], rotX_mat);
        
        translated_tri = rotatedZX_tri;
        translated_tri.p[0].z = rotatedZX_tri.p[0].z + 3.0f;
        translated_tri.p[1].z = rotatedZX_tri.p[1].z + 3.0f;
        translated_tri.p[2].z = rotatedZX_tri.p[2].z + 3.0f;

        mat4x4_dot_vec3(&projected_tri.p[0], &translated_tri.p[0], proj_mat);
        mat4x4_dot_vec3(&projected_tri.p[1], &translated_tri.p[1], proj_mat);
        mat4x4_dot_vec3(&projected_tri.p[2], &translated_tri.p[2], proj_mat);

        /* Scale into view */
        projected_tri.p[0].x += 1.0f; projected_tri.p[0].y += 1.0f;
        projected_tri.p[1].x += 1.0f; projected_tri.p[1].y += 1.0f;
        projected_tri.p[2].x += 1.0f; projected_tri.p[2].y += 1.0f;

        for (int j = 0; j < 3; j++) {
            projected_tri.p[j].x *= 0.5f * (float)WINDOW_WIDTH;
            projected_tri.p[j].y *= 0.5f * (float)WINDOW_HEIGHT;
        }
        
        SDL_DrawTriangle(renderer, projected_tri, white_color);
    } 

    
/* -------------------------------------------------------------------------------------------------- */
    SDL_RenderCopy(renderer, text_texture, NULL, &fps_place);

    SDL_RenderPresent(renderer);
}

void destroy_window(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void fix_framerate(void)
{
    static Uint32 previous_frame_time = 0;
    int time_ellapsed = SDL_GetTicks() - previous_frame_time;
    int time_to_wait = FRAME_TARGET_TIME - time_ellapsed;
    
    if (time_to_wait > 0 && time_to_wait < FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }
    delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0f;
    previous_frame_time = SDL_GetTicks();
}

void print_triangle(triangle t, int padding, char *name)
{
    printf("%s\n", name);
    printf("%*s", padding,""); VEC3_PRINT(t.p[0]);
    printf("%*s", padding,"");VEC3_PRINT(t.p[1]);
    printf("%*s", padding,"");VEC3_PRINT(t.p[2]);
}

void mat4x4_dot_vec3(Vec3 *out, Vec3 *in, Mat m)
{
    out->x = (in->x * MAT_AT(m, 0, 0)) + (in->y * MAT_AT(m, 1, 0)) + (in->z * MAT_AT(m, 2, 0)) + MAT_AT(m, 3, 0); 
    out->y = (in->x * MAT_AT(m, 0, 1)) + (in->y * MAT_AT(m, 1, 1)) + (in->z * MAT_AT(m, 2, 1)) + MAT_AT(m, 3, 1); 
    out->z = (in->x * MAT_AT(m, 0, 2)) + (in->y * MAT_AT(m, 1, 2)) + (in->z * MAT_AT(m, 2, 2)) + MAT_AT(m, 3, 2);
    float w = in->x * MAT_AT(m, 0, 3) + in->y * MAT_AT(m, 1, 3) + in->z * MAT_AT(m, 2, 3) + MAT_AT(m, 3, 3);

    if (w) {
        out->x /= w; 
        out->y /= w;
        out->z /= w;
    }
}

void SDL_DrawTriangle(SDL_Renderer *current_renderer, triangle t, SDL_Color color)
{
    SDL_SetRenderDrawColor(current_renderer, color.r, color.g, color.b, color.a);

    SDL_RenderDrawLineF(renderer, t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y);
    SDL_RenderDrawLineF(renderer, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y);
    SDL_RenderDrawLineF(renderer, t.p[2].x, t.p[2].y, t.p[0].x, t.p[0].y);
}
