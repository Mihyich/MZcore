#ifndef VECTOR2D_H
#define VECTOR2D_H

#ifdef __cplusplus
extern "C" 
{
#endif

#ifdef __linux__
    #define CDECL 
#else
    #define CDECL CDECL
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "LA_sup.h"

#define dot2(x1, y1, x2, y2) ((x1) * (x2) + (y1) * (y2))
#define det2(x1, y1, x2, y2) ((x1) * (y2) - (x2) * (y1))

typedef struct Vector2D vec2;
typedef vec2 *pvec2;
typedef const vec2 cvec2;
typedef cvec2 *cpvec2;

struct Vector2D
{
    float x;
    float y;
};

void CDECL vec2_set(pvec2 v, float x, float y);

void CDECL vec2_invert(pvec2 v);

void CDECL vec2_scale(pvec2 v, float s);

void CDECL vec2_divide(pvec2 v, float d);

void CDECL vec2_add(pvec2 v1, cpvec2 v2);

void CDECL vec2_sub(pvec2 v1, cpvec2 v2);

void CDECL vec2_scaled_add(pvec2 v1, cpvec2 v2, float s);

void CDECL vec2_scaled_sub(pvec2 v1, cpvec2 v2, float s);

void CDECL vec2_add_n(pvec2 v, int n, ...);

void CDECL vec2_sub_n(pvec2 v, int n, ...);

void CDECL vec2_scaled_add_n(pvec2 v, float s, int n, ...);

void CDECL vec2_scaled_sub_n(pvec2 v, float s, int n, ...);

void CDECL vec2_sum(cpvec2 v1, cpvec2 v2, pvec2 res);

void CDECL vec2_diff(cpvec2 v1, cpvec2 v2, pvec2 res);

void CDECL vec2_sum_n(pvec2 res, int n, ...);

void CDECL vec2_component_product(pvec2 v1, cpvec2 v2);

void CDECL vec2_set_component_product(cpvec2 v1, cpvec2 v2, pvec2 res);

float CDECL vec2_dot_product(cpvec2 v1, cpvec2 v2);

float CDECL vec2_det_product(cpvec2 v1, cpvec2 v2);

float CDECL vec2_square_magnitude(cpvec2 v);

float CDECL vec2_magnitude(cpvec2 v);

void CDECL vec2_normalize(pvec2 v);

void CDECL vec2_perpendicular_clw(cpvec2 v, pvec2 res);

void CDECL vec2_perpendicular_nclw(cpvec2 v, pvec2 res);

void CDECL vec2_project(cpvec2 what, cpvec2 where, pvec2 res);

void CDECL vec2_reflect(cpvec2 r, cpvec2 n, pvec2 res);

int CDECL vec2_is_equal(cpvec2 v1, cpvec2 v2);

#ifdef __cplusplus 
}
#endif

#endif // VECTOR2D_H