#include <karbon/math.h>


/* ---------------------------------------------------------------- Matrix -- */


kd_result
kd_mat4_mul(
        const float *a_mat,
        const float *b_mat,
        float *result_mat,
        int count)
{
        (void)a_mat; (void)b_mat;
        (void)result_mat; (void)count;

        return KD_RESULT_FAIL;
}



kd_result
kd_mat4_proj(
        float width,
        float height,
        float near_plane,
        float far_plane,
        float fov_angle)
{
        (void)width; (void)height;
        (void)near_plane; (void)far_plane;
        (void)fov_angle;

        return KD_RESULT_FAIL;
}

