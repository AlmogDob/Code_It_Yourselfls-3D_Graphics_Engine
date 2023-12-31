#ifndef VEC3_H_
#define VEC3_H_

#include <math.h>
#include <stdio.h>
#include <assert.h>

#define VEC3_PRINT(v) Vec3_print(v, #v)

typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec3;

Vec3 Vec3_new(float x, float y, float z);
Vec3 Vec3_add(Vec3 *a,Vec3 *b);
Vec3 Vec3_sub(Vec3 *a,Vec3 *b);
Vec3 Vec3_mul(Vec3 *v, float factor);
Vec3 Vec3_div(Vec3 *v, float factor);
float Vec3_length(Vec3 *v);
float Vec3_dot(Vec3 a, Vec3 b);
Vec3 Vec3_cross(Vec3 *a, Vec3 *b);
void Vec3_normalize(Vec3 *v);
Vec3 Vec3_rotate_around_x(Vec3 v, Vec3 center, float angle);
Vec3 Vec3_rotate_around_y(Vec3 v, Vec3 center, float angle);
Vec3 Vec3_rotate_around_z(Vec3 v, Vec3 center, float angle);
void Vec3_print(Vec3 v, char *name);
void Vec3_init_vec(Vec3 *v);

#endif //VEC3_H_

/* ------------------------------------------------ */

#ifdef VEC3_IMPLEMENTATION

Vec3 Vec3_new(float x, float y, float z)
{
    Vec3 result;
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = 1;
    return result;
}

Vec3 Vec3_add(Vec3 *a,Vec3 *b)
{
    Vec3 result = {
        .x = a->x + b->x,
        .y = a->y + b->y,
        .z = a->z + b->z,
    };
    return result;
}

Vec3 Vec3_sub(Vec3 *a,Vec3 *b)
{
    Vec3 result = {
        .x = a->x - b->x,
        .y = a->y - b->y,
        .z = a->z - b->z,
    };
    return result;
}

Vec3 Vec3_mul(Vec3 *v, float factor)
{
    Vec3 result = {
        .x = v->x * factor,
        .y = v->y * factor,
        .z = v->z * factor,
    };
    return result;
}

Vec3 Vec3_div(Vec3 *v, float factor)
{
    // assert(factor != 0);
    if (factor == 0) {
        factor = 1;
    }
    Vec3 result = {
        .x = v->x / factor,
        .y = v->y / factor,
        .z = v->z / factor,
        .w = v->w / factor,
    };
    return result;
}

float Vec3_length(Vec3 *v)
{
    return sqrtf((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

float Vec3_dot(Vec3 a, Vec3 b)
{
    return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

Vec3 Vec3_cross(Vec3 *a, Vec3 *b)
{
    Vec3 result = {
        .x = (a->y*b->z) - (a->z*b->y),
        .y = (a->z*b->x) - (a->x*b->z),
        .z = (a->x*b->y) - (a->y*b->x),
    };
    return result;
}

void Vec3_normalize(Vec3 *v)
{
    float length = Vec3_length(v);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

Vec3 Vec3_rotate_around_x(Vec3 v, Vec3 center, float angle)
{
    Vec3 rot;
    v.x -= center.x;
    v.y -= center.y;
    v.z -= center.z;
    rot.x = v.x;
    rot.y = v.y * cosf(angle) - v.z * sinf(angle);
    rot.z = v.y * sinf(angle) + v.z * cosf(angle);
    rot.x += center.x;
    rot.y += center.y;
    rot.z += center.z;
    return rot;
}

Vec3 Vec3_rotate_around_y(Vec3 v, Vec3 center, float angle)
{
    Vec3 rot;
    v.x -= center.x;
    v.y -= center.y;
    v.z -= center.z;
    rot.x = v.x * cosf(angle) + v.z * sinf(angle);
    rot.y = v.y;
    rot.z = -v.x * sinf(angle) + v.z * cosf(angle);
    rot.x += center.x;
    rot.y += center.y;
    rot.z += center.z;
    return rot;
}

Vec3 Vec3_rotate_around_z(Vec3 v, Vec3 center, float angle)
{
    Vec3 rot;
    v.x -= center.x;
    v.y -= center.y;
    v.z -= center.z;
    rot.x = v.x * cosf(angle) - v.y * sinf(angle);
    rot.y = v.x * sinf(angle) + v.y * cosf(angle);
    rot.z = v.z;
    rot.x += center.x;
    rot.y += center.y;
    rot.z += center.z;
    return rot;
}

void Vec3_print(Vec3 v, char *name)
{
    printf("%s: (%g, %g, %g)\n", name, v.x, v.y, v.z);
}

void Vec3_init_vec(Vec3 *v)
{
    v->x = 0;
    v->y = 0;
    v->z = 0;
    v->w = 1;
}

#endif //VEC3_IMPLEMENTATION
