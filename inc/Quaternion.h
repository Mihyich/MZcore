#ifndef QUATERNION_H
#define QUATERNION_H

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
#include "Vector3D.h"
#include "Vector4D.h"

typedef struct Quaternion quat;
typedef quat *pquat;
typedef const quat cquat;
typedef cquat *cpquat;

struct Quaternion
{
    float x;
    float y;
    float z;
    float w;
};

void CDECL quat_set(pquat q, float x, float y, float z, float w);

float CDECL quat_square_magnitude(cpquat q);

float CDECL quat_magnitude(cpquat q);

void CDECL quat_normalize(pquat q);

void CDECL quat_conjugate(pquat q);

void CDECL quat_inverse(pquat q);

void CDECL quat_compose(pquat q1, cpquat q2);

void CDECL quat_compose_res(cpquat q1, cpquat q2, pquat res);

void CDECL quat_set_rotate(pquat q, float x, float y, float z, float rad);

void CDECL quat_set_rotate_degrees(pquat q, float x, float y, float z, float degrees);

void CDECL quat_rotate_vec(pvec3 v, cpquat q);

#ifdef __cplusplus
}
#endif

#endif // QUATERNION_H