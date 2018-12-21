/*
 *  Karbon Math
 *  -----------
 *  Unlike most math libraries karbon math is no math types. Its is expected
 *  that the calling code provides suitable sized types.
 *  The reason for this is becasue we don't want to assume too much for the
 *  calling code, maybe you have your own type, maybe you don't want to flip
 *  flop between types etc.
 *
 *  So with that in mind be aware that you need to provide the correct sized
 *  array.
 *
 *  When we ask for the following math constructs we are expecting them to be
 *  equivilant to the following
 *
 *  vec2 == float[2];
 *  vec3 == float[3];
 *  vec4 == float[4];
 *
 *  mat2 == float[4];
 *  mat3 == float[9];
 *  mat4 == float[16];
 *
 *  quat == float[4];
 *  quat_small == float[3];
 */
#ifndef KARBON_MATH
#define KARBON_MATH


#include <karbon/drive.h>


/* ---------------------------------------------------------------- Matrix -- */


kd_result
kd_mat4_mul(
        const float *a_mat,                     /* required */
        const float *b_mat,                     /* required */
        float *result_mat,                      /* required */
        int count);                             /* must be greater than 0 */



kd_result
kd_mat4_proj(
        float width,                            /* greater than 0 */
        float height,                           /* greater than 0 */
        float near_plane,                       /* greater than 0 */
        float far_plane,                        /* greater than near plane */
        float fov_angle);                       /* radians */


/* inc guard */
#endif
