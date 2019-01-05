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


#ifdef __cplusplus
extern "C" {
#endif


#define KDM_PI 3.142
#define KDM_TAU 2 * KDM_PI


/* ---------------------------------------------------------------- Vector -- */


kd_result
kdm_vec3_cross(
        const float *a,
        const float *b,
        float *r);


kd_result
kdm_vec3_dot(
        const float *a,
        const float *b,
        float *out);


kd_result
kdm_vec3_sub(
        const float *a,
        const float *b,
        float *out);


kd_result
kdm_vec3_norm(
        float *in_out);


/* ---------------------------------------------------------------- Matrix -- */


kd_result
kd_mat4_mul(
        const float *a,                         /* required float[16] */
        const float *b,                         /* required float[16] */
        float *result_mat);                     /* must be greater than 0, cannot be a or b */


kd_result
kdm_mat4_id(
        float *out_mat4);



kd_result
kdm_mat4_lookat(
        const float *eye_v3,
        const float *at_v3,
        const float *up_v3,
        float *out_mat4);


kd_result
kdm_mat4_perspective_projection(
        float aspect_ratio,                     /* greater than 0 */
        float near_plane,                       /* greater than 0 */
        float far_plane,                        /* greater than near plane */
        float fov_rads,                         /* radians */
        float *out);                            /* output float[16] */


#ifdef __cplusplus
} /* extern */
#endif


/* inc guard */
#endif
