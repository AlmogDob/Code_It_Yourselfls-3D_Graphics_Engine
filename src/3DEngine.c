/* This code is heavily inspired by the 3D engine of 'OneLoneCoder' in C++
you can find the source code in:
https://github.com/OneLoneCoder/Javidx9/tree/master/ConsoleGameEngine/BiggerProjects/Engine3D .
featured in this video of his:
https://youtu.be/ih20l3pJoeU?si=CzQ8rjk5ZEOlqEHN .*/


/**/


#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define MATRIX_IMPLEMENTATION
#include "Matrix.h"

#define VEC3_IMPLEMENTATION
#include "Vec3.h"

#define VEC2_IMPLEMENTATION
#include "Vec2.h"

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

#define MAX_NUM_OF_TRIANGLES 10000
#define MAX_NUM_OF_VERTS 3*MAX_NUM_OF_TRIANGLES


typedef struct {
    Vec3 p[3];
    float mid_z;
    SDL_Color my_color;
} triangle;

typedef struct {
    triangle tris[MAX_NUM_OF_TRIANGLES];
    int num_of_triangles;
} mesh;

#define MESH_TYPE
#include "load_from_object_file.h"

int initialize_window(void);
void setup(void);
void process_input(void);
void update(void);
void render(void);
void destroy_window(void);
void fix_framerate(void);
void print_triangle(triangle t, int padding, char *name);
void mat4x4_dot_vec3(Vec3 *out, Vec3 *in, Mat m);
void mat4x4_mult_vec3(Vec3 *out, Vec3 *in, Mat m);
Vec3 mat4x4_dot_vec3_with_w(Mat m, Vec3 *in);
void SDL_DrawTriangle(SDL_Renderer *current_renderer, triangle t, SDL_Color color);
float edge_cross(Vec2 *a, Vec2 *b, Vec2 *p);
bool is_top_left(Vec2 *start, Vec2 *end);
void triangle_fill(SDL_Renderer *renderer, triangle t, SDL_Color color);
void qsort_tri(triangle v[], int left, int right);
void swap(triangle v[], int i, int j);
void insertion_sort_tri(triangle A[], int size);
void update_rotX_mat(float Angle_rad);
void update_rotY_mat(float Angle_rad);
void update_rotZ_mat(float Angle_rad);
void update_trans_mat(float x, float y, float z);
void update_proj_mat(float fov_deg, float aspect_ratio, float near, float far);

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;
SDL_Surface *text_surface = NULL;
SDL_Texture *text_texture = NULL;
SDL_Rect fps_place;
SDL_Color white_color;
SDL_Color black_color;
SDL_Color tri_color;
int game_is_running = 0;
float delta_time;
float fps = 0;
mesh simple_shape_mesh;
mesh video_ship_mesh;
mesh lego_man_mesh;
mesh bolt_mesh;
mesh teapot_mesh;
mesh *mesh_to_use;
triangle triangles_to_render[MAX_NUM_OF_TRIANGLES];
Mat proj_mat, rotZ_mat, rotY_mat,
rotX_mat, trans_mat, world_mat, temp;
Vec3 camera;
Uint32 previous_frame_time = 0;

int current_MAX_num_of_triangles = 0;
float theta = 0;
int number_of_triangles_to_render = 0;
float Near, Aspect_Ratio, Far, FoV;

int space_bar_was_pressed = 0;
int to_render = 1;
int to_update = 1;


int main()
{
    srand(time(0));
    game_is_running = !initialize_window();

    setup();

    while (game_is_running) {
        process_input();
        if (to_update) {
            update();
        }
        if (to_render) {
            render();
        }
        
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

    // simple_shape_mesh.num_of_triangles = load_from_object_file(&simple_shape_mesh, "./simple_shape/simple_shape.obj");
    // if (!simple_shape_mesh.num_of_triangles) {
    //     fprintf(stderr, "Error loading 'simple shape'.\n");
    //     return -1;
    // }
    
    video_ship_mesh.num_of_triangles = load_from_object_file(&video_ship_mesh, "./obj_files/Video_Ship/Video_Ship.obj");
    if (!video_ship_mesh.num_of_triangles) {
        fprintf(stderr, "Error loading 'video ship'.\n");
        return -1;
    }

    // lego_man_mesh.num_of_triangles = load_from_object_file(&lego_man_mesh, "./lego_man/LEGO_Man2.obj");
    // if (!lego_man_mesh.num_of_triangles) {
    //     fprintf(stderr, "Error loading 'lego man'.\n");
    //     return -1;
    // }

    // bolt_mesh.num_of_triangles = load_from_object_file(&bolt_mesh, "./Bolt.obj");
    // if (!bolt_mesh.num_of_triangles) {
    //     fprintf(stderr, "Error loading 'bolt'.\n");
    //     return -1;
    // }

    teapot_mesh.num_of_triangles = load_from_object_file(&teapot_mesh, "./obj_files/teapot.obj");
    if (!teapot_mesh.num_of_triangles) {
        fprintf(stderr, "Error loading 'teapot'.\n");
        return -1;
    }

    return 0;
}

void setup(void)
{    
    mesh_to_use = &teapot_mesh;

    white_color.a = 255;
    white_color.b = 255;
    white_color.g = 255;
    white_color.r = 255;

    black_color.a = 255;
    black_color.b = 0;
    black_color.g = 0;
    black_color.r = 0;

    tri_color = white_color;

    fps_place.x = 10;
    fps_place.y = 10;
    fps_place.w = 110;
    fps_place.h = 25;

    Near = 0.1f;
    Far = 1000.0f;
    FoV = 90.0f;
    Aspect_Ratio = (float)WINDOW_HEIGHT / (float)WINDOW_WIDTH;
    
    proj_mat = mat_alloc(4,4);
    rotZ_mat = mat_alloc(4,4);
    rotY_mat = mat_alloc(4,4);
    rotX_mat = mat_alloc(4,4);
    trans_mat = mat_alloc(4,4);
    world_mat = mat_alloc(4,4);
    temp = mat_alloc(4,4);

    mat_fill(proj_mat, 0.0f);
    mat_fill(rotZ_mat, 0.0f);
    mat_fill(rotY_mat, 0.0f);
    mat_fill(rotX_mat, 0.0f);
    mat_fill(trans_mat, 0.0f);
    mat_fill(world_mat, 0.0f);
    mat_fill(temp, 0.0f);

    update_proj_mat(FoV, Aspect_Ratio, Near, Far);
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
                if (event.key.keysym.sym == SDLK_SPACE) {
                    if (!space_bar_was_pressed) {
                        to_render = 0;
                        to_update = 0;
                        space_bar_was_pressed = 1;
                        break;
                    }
                    if (space_bar_was_pressed) {
                        to_render = 1;
                        to_update = 1;
                        previous_frame_time = SDL_GetTicks();
                        space_bar_was_pressed = 0;
                        break;
                    }
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

    update_rotZ_mat(theta);
    update_rotX_mat(theta * 0.5f);
    update_trans_mat(0.0f, 0.0f, 16.0f);
    // update_proj_mat(FoV, Aspect_Ratio, Near, Far);
    
    mat_fill(world_mat, 0.0f);
    mat_fill(temp, 0.0f);
    
    mat_dot(world_mat, rotZ_mat, rotX_mat);
    mat_dot(temp, world_mat, trans_mat);
    world_mat = temp;

    /* Create Triangles */
    triangle projected_tri, translated_tri, rotatedZ_tri, rotatedZX_tri, transformed_tri, tri;
    Vec3 normal, line1, line2, light_position;
    Vec3 offset_view = {1,1,0,0};
    float dp;
    number_of_triangles_to_render = 0;
    for (int i = 0; i < mesh_to_use->num_of_triangles; i++) {
        
        tri = mesh_to_use->tris[i];
        
        transformed_tri.p[0] = mat4x4_dot_vec3_with_w(world_mat, &tri.p[0]);
        transformed_tri.p[1] = mat4x4_dot_vec3_with_w(world_mat, &tri.p[1]);
        transformed_tri.p[2] = mat4x4_dot_vec3_with_w(world_mat, &tri.p[2]);

        /*Calculate triangle normal */
        line1 = Vec3_sub(&transformed_tri.p[1], &transformed_tri.p[0]);
        line2 = Vec3_sub(&transformed_tri.p[2], &transformed_tri.p[0]);

        normal = Vec3_cross(&line1, &line2);
        Vec3_normalize(&normal);

        if (Vec3_dot(normal, Vec3_sub(&transformed_tri.p[0], &camera)) < 0.0f) {
            
            /* Illumination */
            tri_color = white_color;
            light_position = Vec3_new(0.0f, 0.0f, -1.0f);
            Vec3_normalize(&light_position);
            
            dp = Vec3_dot(normal, light_position);
            if (dp < 0) {
                dp = 0;
            }
            
            tri_color.r *= dp;
            tri_color.g *= dp;
            tri_color.b *= dp;

            projected_tri.p[0] = mat4x4_dot_vec3_with_w(proj_mat, &transformed_tri.p[0]);
            projected_tri.p[1] = mat4x4_dot_vec3_with_w(proj_mat, &transformed_tri.p[1]);
            projected_tri.p[2] = mat4x4_dot_vec3_with_w(proj_mat, &transformed_tri.p[2]);

            projected_tri.p[0] = Vec3_div(&projected_tri.p[0], projected_tri.p[0].w);
            projected_tri.p[1] = Vec3_div(&projected_tri.p[1], projected_tri.p[1].w);
            projected_tri.p[2] = Vec3_div(&projected_tri.p[2], projected_tri.p[2].w);

            /* Scale into view */
            projected_tri.p[0] = Vec3_add(&projected_tri.p[0], &offset_view); 
            projected_tri.p[1] = Vec3_add(&projected_tri.p[1], &offset_view); 
            projected_tri.p[2] = Vec3_add(&projected_tri.p[2], &offset_view); 

            for (int j = 0; j < 3; j++) {
                projected_tri.p[j].x *= 0.5f * (float)WINDOW_WIDTH;
                projected_tri.p[j].y *= 0.5f * (float)WINDOW_HEIGHT;
            }
            projected_tri.mid_z = (projected_tri.p[0].z + projected_tri.p[1].z + projected_tri.p[2].z) / 3.0f;
            projected_tri.my_color = tri_color;
            triangles_to_render[number_of_triangles_to_render] = projected_tri;
            number_of_triangles_to_render++;
            // triangle_fill(renderer, projected_tri, tri_color);
            // SDL_DrawTriangle(renderer, projected_tri, white_color);

        }
    } 

}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 0x1E, 0x1E, 0x1E, 255);
    SDL_RenderClear(renderer);
/* -------------------------------------------------------------------------------------------------- */
    // qsort_tri(triangles_to_render, 0, number_of_triangles_to_render);
    insertion_sort_tri(triangles_to_render, number_of_triangles_to_render);
    
    // dprintINT(number_of_triangles_to_render);
    for (int i = 0; i < number_of_triangles_to_render; i++) {
        triangle_fill(renderer, triangles_to_render[i], triangles_to_render[i].my_color);
        // SDL_DrawTriangle(renderer, triangles_to_render[i], white_color);

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

void mat4x4_mult_vec3(Vec3 *out, Vec3 *in, Mat m)
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

Vec3 mat4x4_dot_vec3_with_w(Mat m, Vec3 *in)
{
    Vec3 v = {
        v.x = (in->x * MAT_AT(m, 0, 0)) + (in->y * MAT_AT(m, 1, 0)) + (in->z * MAT_AT(m, 2, 0)) + (in->w * MAT_AT(m, 3, 0)), 
        v.y = (in->x * MAT_AT(m, 0, 1)) + (in->y * MAT_AT(m, 1, 1)) + (in->z * MAT_AT(m, 2, 1)) + (in->w * MAT_AT(m, 3, 1)), 
        v.z = (in->x * MAT_AT(m, 0, 2)) + (in->y * MAT_AT(m, 1, 2)) + (in->z * MAT_AT(m, 2, 2)) + (in->w * MAT_AT(m, 3, 2)),
        v.w = (in->x * MAT_AT(m, 0, 3)) + (in->y * MAT_AT(m, 1, 3)) + (in->z * MAT_AT(m, 2, 3)) + (in->w * MAT_AT(m, 3, 3)),
    };
    return v;
}

void SDL_DrawTriangle(SDL_Renderer *current_renderer, triangle t, SDL_Color color)
{
    SDL_SetRenderDrawColor(current_renderer, color.r, color.g, color.b, color.a);

    SDL_RenderDrawLineF(renderer, t.p[0].x, WINDOW_HEIGHT - t.p[0].y, t.p[1].x, WINDOW_HEIGHT - t.p[1].y);
    SDL_RenderDrawLineF(renderer, t.p[1].x, WINDOW_HEIGHT - t.p[1].y, t.p[2].x, WINDOW_HEIGHT - t.p[2].y);
    SDL_RenderDrawLineF(renderer, t.p[2].x, WINDOW_HEIGHT - t.p[2].y, t.p[0].x, WINDOW_HEIGHT - t.p[0].y);
}

float edge_cross(Vec2 *a, Vec2 *b, Vec2 *p)
{
    Vec2 ab = {b->x - a->x, b->y - a->y};
    Vec2 ap = {p->x - a->x, p->y - a->y};
    return (int)Vec2_cross(&ab, &ap);
}

bool is_top_left(Vec2 *start, Vec2 *end)
{
    Vec2 edge = {end->x - start->x, end->y - start->y};

    bool is_top_edge = edge.y == 0 && edge.x > 0;
    bool is_left_edge = edge.y < 0;

    return is_top_edge || is_left_edge;
}

void triangle_fill(SDL_Renderer *renderer, triangle t, SDL_Color color)
{
    Vec2 v0, v1, v2;

    v0.x = t.p[0].x;
    v0.y = t.p[0].y;

    v1.x = t.p[2].x;
    v1.y = t.p[2].y;

    v2.x = t.p[1].x;
    v2.y = t.p[1].y;
    
    // find the bounding box with all candidate pixels
    int x_min = floor(MIN(MIN(v0.x, v1.x), v2.x));
    int y_min = floor(MIN(MIN(v0.y, v1.y), v2.y));
    int x_max = ceil(MAX(MAX(v0.x, v1.x), v2.x));
    int y_max = ceil(MAX(MAX(v0.y, v1.y), v2.y));

    float delta_w0_col = (v1.y - v2.y);
    float delta_w1_col = (v2.y - v0.y);
    float delta_w2_col = (v0.y - v1.y);
    
    float delta_w0_row = (v2.x - v1.x);
    float delta_w1_row = (v0.x - v2.x);
    float delta_w2_row = (v1.x - v0.x);

    // float area = edge_cross(&v0, &v1, &v2);

    float bias0 = is_top_left(&v1, &v2) ? 0 : -1e-10;
    float bias1 = is_top_left(&v2, &v0) ? 0 : -1e-10;
    float bias2 = is_top_left(&v0, &v1) ? 0 : -1e-10;

    Vec2 p0 = {x_min + 0.5f, y_min + 0.5f};
    float w0_row = edge_cross(&v1, &v2, &p0) + bias0;
    float w1_row = edge_cross(&v2, &v0, &p0) + bias1;
    float w2_row = edge_cross(&v0, &v1, &p0) + bias2;

    // loop all candidate pixels inside the bounding box
    for (int y = y_min; y <= y_max; y++) {

        float w0 = w0_row;
        float w1 = w1_row;
        float w2 = w2_row;

        for ( int x = x_min; x <= x_max; x++) {
            
            bool is_inside = (w0 >= 0 && w1 >= 0 && w2 >= 0);
            
            if (is_inside) {
                // float alpha = (float)w0 / area;
                // float beta = (float)w1 / area;
                // float gamma = (float)w2 / area;

                // draw_pixel(x, y, color);
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_RenderDrawPoint(renderer, x, WINDOW_HEIGHT - y);
            }
            w0 += delta_w0_col;
            w1 += delta_w1_col;
            w2 += delta_w2_col;
        }
        w0_row += delta_w0_row;
        w1_row += delta_w1_row;
        w2_row += delta_w2_row;
    }
}

/* qsort_tri: sort v[left]...v[right] int increasing order */
void qsort_tri(triangle v[], int left, int right)
{
    int i, last;

    if (left >= right)                  /* do nothing if array contains */
        return;                         /* fewer than two elements */
    swap(v, left, (left + right) / 2);  /* move partition elem */
    last = left;                        /* to v[0] */
    for (i = left + 1; i <= right; i++) /* partition */
        if (v[i].mid_z > v[left].mid_z)
            swap(v, ++last, i);
    swap(v, left, last); /* restore partition elem */
    qsort_tri(v, left, last - 1);
    qsort_tri(v, last + 1, right);
}

/* swap: interchange v[i] and v[j] */
void swap(triangle v[], int i, int j)
{
    triangle temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

void insertion_sort_tri(triangle A[], int size)
{
    triangle value;
    int hole;

    for (int i = 0; i < size; i++) {
        value = A[i];
        hole = i;
        while (hole > 0 && A[hole-1].mid_z < value.mid_z) {
            A[hole] = A[hole - 1];
            hole--;
        }
        A[hole] = value;
    }
}

void update_rotX_mat(float Angle_rad)
{
    /* Rotation X Matrix */
    MAT_AT(rotX_mat, 0, 0) = 1.0f;
    MAT_AT(rotX_mat, 1, 1) = cosf(Angle_rad);
    MAT_AT(rotX_mat, 1, 2) = sinf(Angle_rad);
    MAT_AT(rotX_mat, 2, 1) = -sinf(Angle_rad);
    MAT_AT(rotX_mat, 2, 2) = cosf(Angle_rad);
    MAT_AT(rotX_mat, 3, 3) = 1.0f;
}

void update_rotY_mat(float Angle_rad)
{
    /* Rotation Y Matrix */
    MAT_AT(rotY_mat, 0, 0) = cosf(Angle_rad);
    MAT_AT(rotY_mat, 0, 2) = sinf(Angle_rad);
    MAT_AT(rotY_mat, 2, 0) = -sinf(Angle_rad);
    MAT_AT(rotY_mat, 1, 1) = 1.0f;
    MAT_AT(rotY_mat, 2, 2) = cosf(Angle_rad);
    MAT_AT(rotY_mat, 3, 3) = 1.0f;
}

void update_rotZ_mat(float Angle_rad)
{
    /* Rotation Z Matrix */
    MAT_AT(rotZ_mat, 0, 0) = cosf(Angle_rad);
    MAT_AT(rotZ_mat, 0, 1) = sinf(Angle_rad);
    MAT_AT(rotZ_mat, 1, 0) = -sinf(Angle_rad);
    MAT_AT(rotZ_mat, 1, 1) = cosf(Angle_rad);
    MAT_AT(rotZ_mat, 2, 2) = 1.0f;
    MAT_AT(rotZ_mat, 3, 3) = 1.0f;
}

void update_trans_mat(float x, float y, float z)
{
    MAT_AT(trans_mat, 0, 0) = 1.0f;
    MAT_AT(trans_mat, 1, 1) = 1.0f;
    MAT_AT(trans_mat, 2, 2) = 1.0f;
    MAT_AT(trans_mat, 3, 3) = 1.0f;
    MAT_AT(trans_mat, 3, 0) = x;
    MAT_AT(trans_mat, 3, 1) = y;
    MAT_AT(trans_mat, 3, 2) = z;
}

void update_proj_mat(float fov_deg, float aspect_ratio, float near, float far)
{
    float fov_rad = 1.0f / tanf((fov_deg * 0.5f *PI)/ 180.0f);
    MAT_AT(proj_mat, 0, 0) = aspect_ratio * fov_rad;
    MAT_AT(proj_mat, 1, 1) = fov_rad;
    MAT_AT(proj_mat, 2, 2) = far / (far - near);
    MAT_AT(proj_mat, 3, 2) = (-far * near)/(far - near);
    MAT_AT(proj_mat, 2, 3) = 1.0f;
    MAT_AT(proj_mat, 3, 3) = 0.0f;
}
