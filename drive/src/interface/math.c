#include <karbon/math.h>
#include "../config.h"
#include <math.h>


/* -------------------------------------------------------------- Vector 3 -- */


kd_result
kdm_vec3_cross(
        const float *a,
        const float *b,
        float *out)
{
        if (KD_PCHECK && (!out || !a || !b)) {
                KD_ASSERT(!"KD_RESULT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }

        float x = (a[1] * b[2]) - (a[2] * b[1]);
        float y = (a[0] * b[2]) - (a[2] * b[0]);
        float z = (a[0] * b[1]) - (a[1] * b[0]);

        out[0] = x; out[1] = -y; out[2] = z;

        return KD_RESULT_OK;
}


kd_result
kdm_vec3_dot(
        const float *a,
        const float *b,
        float *out)
{
        if(KD_PCHECK && (!out || !a || !b)) {
                KD_ASSERT(!"KD_RESULT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }

        float x = a[0] * b[0];
        float y = a[1] * b[1];
        float z = a[2] * b[2];
        float dot = x + y + z;

        *out = dot;

        return KD_RESULT_OK;
}


kd_result
kdm_vec3_norm(
        float *in_out)
{
        if(KD_PCHECK && !in_out) {
                KD_ASSERT(!"KD_RESULT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }

        float sq_len = (in_out[0] * in_out[0])
                  + (in_out[1] * in_out[1])
                  + (in_out[2] * in_out[2]);

        float len = sqrtf(sq_len);
        len = 1.f / len;

        in_out[0] *= len;
        in_out[1] *= len;
        in_out[2] *= len;

        return KD_RESULT_OK;
}


kd_result
kdm_vec3_sub(
        const float *a,
        const float *b,
        float *out)
{
        if(KD_PCHECK && (!out || !a || !b)) {
                KD_ASSERT(!"KD_RESULT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }

        out[0] = a[0] - b[0];
        out[1] = a[1] - b[1];
        out[2] = a[2] - b[2];

        return KD_RESULT_OK;
}


/* -------------------------------------------------------------- Vector 4 -- */

  
kd_result
kdm_vec4_dot(
        const float *a,
        const float *b,
        float *out)
{
        if(KD_PCHECK && (!out || !a || !b)) {
                KD_ASSERT(!"KD_RESULT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }

        float x = a[0] * b[0];
        float y = a[1] * b[1];
        float z = a[2] * b[2];
        float w = a[3] * b[3];
        float dot = x + y + z + w;

        *out = dot;

        return KD_RESULT_OK;
}


/* ---------------------------------------------------------------- Matrix -- */


kd_result
kdm_mat4_id(
        float *out)
{
        if(KD_PCHECK && (!out)) {
                KD_ASSERT(!"KD_RESULT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }

        out[0]  = 1.f; out[1]  = 0.f; out[2]  = 0.f; out[3]  = 0.f;
        out[4]  = 0.f; out[5]  = 1.f; out[6]  = 0.f; out[7]  = 0.f;
        out[8]  = 0.f; out[9]  = 0.f; out[10] = 1.f; out[11] = 0.f;
        out[12] = 0.f; out[13] = 0.f; out[14] = 0.f; out[15] = 1.f;

        return KD_RESULT_OK;
}


kd_result
kdm_mat4_lookat(
        const float *eye,
        const float *at,
        const float *up,
        float *out)
{
        if (KD_PCHECK && (!out || !eye || !at || !up)) {
                KD_ASSERT(!"KD_RESULT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }
        
        float xax[4]; float yax[4]; float zax[4];
        float wax[4] = {0.f, 0.f, 0.f, 1.f};

        kdm_vec3_sub(eye, at, zax);
        kdm_vec3_norm(zax);

        kdm_vec3_cross(up, zax, xax);
        kdm_vec3_norm(xax);

        kdm_vec3_cross(zax, xax, yax);
        kdm_vec3_norm(yax);

        kdm_vec3_dot(xax, eye, &xax[3]);
        kdm_vec3_dot(yax, eye, &yax[3]);
        kdm_vec3_dot(zax, eye, &zax[3]);

        out[0]  = +xax[0]; out[1]  = +yax[0]; out[2]  = +zax[0]; out[3]  = +wax[0];
        out[4]  = +xax[1]; out[5]  = +yax[1]; out[6]  = +zax[1]; out[7]  = +wax[1];
        out[8]  = +xax[2]; out[9]  = +yax[2]; out[10] = +zax[2]; out[11] = +wax[2];
        out[12] = -xax[3]; out[13] = -yax[3]; out[14] = -zax[3]; out[15] = +wax[3];

        return KD_RESULT_OK;
}


kd_result
kdm_mat4_perspective_projection(
        float aratio,
        float nplane,
        float fplane,
        float fov,
        float *out)
{
        if (KD_PCHECK && ((nplane > fplane) || (fov == 0.f) || (!out))) {
                KD_ASSERT(!"KD_RESULT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }

        float pdiff = fplane - nplane;

        float b = 1.f / tanf(fov * 0.5f);
        float a = b / aratio;
        float c = -(fplane + nplane) / pdiff;
        float d = -(2.f * fplane * nplane) / pdiff;

        out[0]  = a;   out[1]  = 0.f; out[2]  = 0.f; out[3]  = 0.f;
        out[4]  = 0.f; out[5]  = b;   out[6]  = 0.f; out[7]  = 0.f;
        out[8]  = 0.f; out[9]  = 0.f; out[10] = c;   out[11] = -1.f;
        out[12] = 0.f; out[13] = 0.f; out[14] = d;   out[15] = 0.f;

        /*
        float y = 1.0f / tanf(fov * 0.5f);
        float x = y / aratio;

        float a = 1.0f;
        float b = -2.0f * nplane;

        out[ 0] =    x; out[ 1] = 0.0f; out[ 2] = 0.0f; out[ 3] = 0.0f;
        out[ 4] = 0.0f; out[ 5] =    y; out[ 6] = 0.0f; out[ 7] = 0.0f;
        out[ 8] = 0.0f; out[ 9] = 0.0f; out[10] =    a; out[11] =    b;
        out[12] = 0.0f; out[13] = 0.0f; out[14] = 1.0f; out[15] = 0.0f;
        */

        return KD_RESULT_OK;
}


kd_result
kdm_mat4_multiply(
        const float *a,
        const float *b,
        float *out)
{
        if(KD_PCHECK && (!a || !b || !out)) {
                KD_ASSERT(!"KD_RESULT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }

        
        int i;
        for(i = 0; i < 16; ++i) {
                int row = (i / 4) * 4;
                int col = (i % 4);

                const float *row_vec = &a[row];

                float col_vec[4];
                col_vec[0] = b[col + 0];
                col_vec[1] = b[col + 1];
                col_vec[2] = b[col + 2];
                col_vec[3] = b[col + 3];

                kdm_vec4_dot(row_vec, col_vec, &out[i]);
        }

        return KD_RESULT_OK;
}
