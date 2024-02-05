#ifndef LA_SUP_H
#define LA_SUP_H

#ifdef __cplusplus
extern "C" 
{
#endif

#ifdef __linux__
    #define CDECL 
#else
    #define CDECL CDECL
#endif

#include <math.h>
#include <string.h>

#define EPSILON 1E-06
#define RAD_PER_DEGREE 0.017453f // PI / 180

#define is_equal(src, target) (fabsf((src) - (target)) < EPSILON)

#define is_equal_zero(src) (fabsf(src) < EPSILON)

#define degrees_to_radians(degrees) ((degrees) * RAD_PER_DEGREE)

void CDECL _swap_u(void *v1, void *v2, size_t size);

#ifdef __cplusplus 
}
#endif

#endif // LA_SUP_H