#ifndef MATRIX2D_H
#define MATRIX2D_H

#ifdef __cplusplus
extern "C" 
{
#endif

#ifdef __linux__
    #define CDECL 
#else
    #define CDECL CDECL
#endif

#include "LA_sup.h"
#include "Vector2D.h"

typedef struct Matrix2D mat2;
typedef mat2 *pmat2;
typedef const mat2 cmat2;
typedef cmat2 *cpmat2;

struct Matrix2D
{
    vec2 x_basis;
    vec2 y_basis;
};

void CDECL mat2_set(pmat2 m, float xx, float xy, float yx, float yy);

void CDECL mat2_set_x_basis(pmat2 m, float x, float y);

void CDECL mat2_set_y_basis(pmat2 m, float x, float y);

void CDECL mat2_set_basises(pmat2 m, cpvec2 x, cpvec2 y);

void CDECL mat2_transpose(pmat2 m);

void CDECL mat2_scalar_mult(pmat2 m, float s);

void CDECL mat2_divide(pmat2 m, float d);

void CDECL mat2_add(pmat2 m1, cpmat2 m2);

void CDECL mat2_sub(pmat2 m1, cpmat2 m2);

void CDECL mat2_sum(cpmat2 m1, cpmat2 m2, pmat2 res);

void CDECL mat2_diff(cpmat2 m1, cpmat2 m2, pmat2 res);

void CDECL mat2_compose(pmat2 m1, cpmat2 m2);

void CDECL mat2_transform(cpmat2 m, pvec2 v);

void CDECL mat2_compose_res(cpmat2 m1, cpmat2 m2, pmat2 res);

void CDECL mat2_transform_res(cpmat2 m, cpvec2 v, pvec2 res);

float CDECL mat2_determinant(cpmat2 m);

void CDECL mat2_inverse(pmat2 m);

void CDECL mat2_normalize(pmat2 m);

void CDECL mat2_set_scale(pmat2 m, float x, float y);

void CDECL mat2_set_rotate(pmat2 m, float rad);

void CDECL mat2_set_rotate_degrees(pmat2 m, float degrees);

void CDECL mat2_scale(pmat2 m, float x, float y);

void CDECL mat2_rotate(pmat2 m, float rad);

void CDECL mat2_rotate_degrees(pmat2 m, float degrees);

int CDECL mat2_is_equal(cpmat2 m1, cpmat2 m2);

#ifdef __cplusplus 
}
#endif

#endif // MATRIX2D_H