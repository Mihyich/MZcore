#ifndef VECTOR3D_H
#define VECTOR3D_H

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
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "LA_sup.h"

#define dot3(x1, y1, z1, x2, y2, z2) ((x1) * (x2) + (y1) * (y2) + (z1) * (z2))
#define det3(x1, y1, z1, x2, y2, z2, x3, y3, z3) \
            ((x1) * ((y2) * (z3) - (y3) * (z2)) - \
            (y1) * ((x2) * (z3) - (x3) * (z2)) + \
            (z1) * ((x2) * (y3) - (x3) * (y2)))

typedef struct Vector3D vec3;
typedef vec3 *pvec3;
typedef const vec3 cvec3;
typedef cvec3 *cpvec3;

struct Vector3D
{
    float x;
    float y;
    float z;
};

void CDECL vec3_set(pvec3, float x, float y, float z);

void CDECL vec3_invert(pvec3 v);

void CDECL vec3_scale(pvec3 v, float s);

void CDECL vec3_divide(pvec3 v, float d);

void CDECL vec3_add(pvec3 v1, cpvec3 v2);

void CDECL vec3_sub(pvec3 v1, cpvec3 v2);

void CDECL vec3_scaled_add(pvec3 v1, cpvec3 v2, float s);

void CDECL vec3_scaled_sub(pvec3 v1, cpvec3 v2, float s);

void CDECL vec3_add_n(pvec3 v, int n, ...);

void CDECL vec3_sub_n(pvec3 v, int n, ...);

void CDECL vec3_scaled_add_n(pvec3 v, float s, int n, ...);

void CDECL vec3_scaled_sub_n(pvec3 v, float s, int n, ...);

void CDECL vec3_sum(cpvec3 v1, cpvec3 v2, pvec3 res);

void CDECL vec3_diff(cpvec3 v1, cpvec3 v2, pvec3 res);

void CDECL vec3_sum_n(pvec3 res, int n, ...);

void CDECL vec3_component_product(pvec3 v1, cpvec3 v2);

void CDECL vec3_set_component_product(cpvec3 v1, cpvec3 v2, pvec3 res);

float CDECL vec3_dot_product(cpvec3 v1, cpvec3 v2);

float CDECL vec3_det_product(cpvec3 v1, cpvec3 v2, cpvec3 v3);

float CDECL vec3_square_magnitude(cpvec3 v);

float CDECL vec3_magnitude(cpvec3 v);

void CDECL vec3_vector_product(cpvec3 v1, cpvec3 v2, pvec3 res);

void CDECL vec3_normalize(pvec3 v);

void CDECL vec3_project(cpvec3 what, cpvec3 where, pvec3 res);

void CDECL vec3_reflect(cpvec3 r, cpvec3 n, pvec3 res);

int CDECL vec3_is_equal(cpvec3 v1, cpvec3 v2);

#ifdef __cplusplus 
}
#endif

#endif // VECTOR3D_H