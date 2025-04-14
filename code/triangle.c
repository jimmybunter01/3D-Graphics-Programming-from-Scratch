#include "display.h"
#include "triangle.h"
#include "colours.h "
#include "texture.h"

void int_swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void float_swap(float* a, float* b) {
    float tmp = *a;
    *a = *b;
    *b = tmp;
}

void triangle_swap(triangle_t *a, triangle_t *b) {
    triangle_t tmp = *a;
    *a = *b;
    *b = tmp;
}

// void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t colour) {
//     float inverted_slope_1 = (float)(x1 - x0) / (y1 - y0);
//     float inverted_slope_2 = (float)(x2 - x0) / (y2 - y0);

//     float x_start = x0;
//     float x_end = x0;

//     for (int y = y0; y <= y2; y++) {
//         draw_line(x_start, x_end, y, y, colour);
//         x_start += inverted_slope_1;
//         x_end += inverted_slope_2;
//     }
// }

// void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t colour) {
//     float inverted_slope_1 = (float)(x2 - x0) / (y2 - y0);
//     float inverted_slope_2 = (float)(x2 - x1) / (y2 - y1);

//     float x_start = x2;
//     float x_end = x2;

//     for (int y = y2; y >= y1; y--) {
//         draw_line(x_start, x_end, y, y, colour);
//         x_start -= inverted_slope_1;
//         x_end -= inverted_slope_2;
//     }
// }

void draw_filled_triangle(triangle_t triangle, uint32_t colour) {
    int x0 = triangle.points[0].x;
    int y0 = triangle.points[0].y;
    float z0 = triangle.points[0].z;
    float w0 = triangle.points[0].w;

    int x1 = triangle.points[1].x;
    int y1 = triangle.points[1].y;
    float z1 = triangle.points[1].z;
    float w1 = triangle.points[1].w;

    int x2 = triangle.points[2].x;
    int y2 = triangle.points[2].y;
    float z2 = triangle.points[2].z;
    float w2 = triangle.points[2].w;

    // Sort Vertices to the following order (y0 < y1 < y2)
    if (y0 > y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&z0, &z1);
        float_swap(&w0, &w1);
    }
    if (y1 > y2) {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
        float_swap(&z1, &z2);
        float_swap(&w1, &w2);
    }
    if (y0 > y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&z0, &z1);
        float_swap(&w0, &w1);
    }

    vec4_t point_a = {x0, y0, z0, w0};
    vec4_t point_b = {x1, y1, z1, w1};
    vec4_t point_c = {x2, y2, z2, w2};

    // Flat-Bottom Triangle
    float inverted_slope_1 = 0;
    float inverted_slope_2 = 0;

    if (y1 - y0 != 0) inverted_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
    if (y2 - y0 != 0) inverted_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

    if (y1 - y0 != 0) {
        for (int y = y0; y <= y1; y++) {
            int x_start = x1 + (y - y1) * inverted_slope_1;
            int x_end = x0 + (y - y0) * inverted_slope_2;

            if (x_end < x_start) int_swap(&x_start, &x_end);

            for (int x = x_start; x < x_end; x++) {
                draw_colour_fill(x, y, point_a, point_b, point_c, colour);
            }
        }
    }

    // Flat-Top Triangle
    if (y2 - y1 != 0) inverted_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
    if (y2 - y0 != 0) inverted_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

    if (y2 - y1 != 0) {
        for (int y = y1; y <= y2; y++) {
            int x_start = x1 + (y - y1) * inverted_slope_1;
            int x_end = x0 + (y - y0) * inverted_slope_2;

            if (x_end < x_start) int_swap(&x_start, &x_end);

            for (int x = x_start; x < x_end; x++) {
                draw_colour_fill(x, y, point_a, point_b, point_c, colour);
            }
        }
    }
}
vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p) {
    vec2_t ac = vec2_subtract(c, a);
    vec2_t ab = vec2_subtract(b, a);
    vec2_t ap = vec2_subtract(p, a);
    vec2_t pc = vec2_subtract(c, p);
    vec2_t pb = vec2_subtract(b, p);

    float area_parallelogram_abc = (ac.x * ab.y - ac.y * ab.x); // ||AC x AB||

    float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;
    float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;
    float gamma = 1 - beta - alpha;

    vec3_t weights = {alpha, beta, gamma};
    return weights;
}


void draw_colour_fill(int x, int y, vec4_t point_a, vec4_t point_b, vec4_t point_c, uint32_t colour) {
    vec2_t point_p = {x,y};
    vec2_t a = vec2_from_vec4(point_a);
    vec2_t b = vec2_from_vec4(point_b);
    vec2_t c = vec2_from_vec4(point_c);

    vec3_t weights = barycentric_weights(a, b, c, point_p);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    float interpolated_reciprocal_w = alpha * (1 / point_a.w) + beta * (1 / point_b.w) + gamma * (1 / point_c.w);

    // 1/w needs to adjusted so that the vlaues which are closer to the camera have smaller values.
    interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

    if (interpolated_reciprocal_w < z_buffer[(window_width * y) + x]) {
        draw_pixel(x, y, colour);
        z_buffer[(window_width * y) + x] = interpolated_reciprocal_w;
    }
}


void draw_texture(
    int x, int y
    , vec4_t point_a, vec4_t point_b, vec4_t point_c
    , tex2_t a_uv, tex2_t b_uv, tex2_t c_uv
    , uint32_t* texture
) {
    vec2_t point_p = {x,y};
    vec2_t a = vec2_from_vec4(point_a);
    vec2_t b = vec2_from_vec4(point_b);
    vec2_t c = vec2_from_vec4(point_c);

    vec3_t weights = barycentric_weights(a, b, c, point_p);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    float interpolated_u;
    float interpolated_v;
    float interpolated_reciprocal_w;

    interpolated_u = alpha * (a_uv.u / point_a.w) + beta * (b_uv.u / point_b.w) + gamma * (c_uv.u / point_c.w);
    interpolated_v = alpha * (a_uv.v / point_a.w) + beta * (b_uv.v / point_b.w) + gamma * (c_uv.v / point_c.w);
    interpolated_reciprocal_w = alpha * (1 / point_a.w) + beta * (1 / point_b.w) + gamma * (1 / point_c.w);

    interpolated_u /= interpolated_reciprocal_w;
    interpolated_v /= interpolated_reciprocal_w;

    // Modulus ensures that the values are never < 0, so outside the the triangle the texture is drawn to.
    int texture_x = abs((int)(interpolated_u * texture_width)) % texture_width;
    int texture_y = abs((int)(interpolated_v * texture_height)) % texture_height;

    // 1/w needs to adjusted so that the vlaues which are closer to the camera have smaller values.
    interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

    if (interpolated_reciprocal_w < z_buffer[(window_width * y) + x]) {
        draw_pixel(x,y, texture[(texture_width * texture_y) + texture_x]);
        z_buffer[(window_width * y) + x] = interpolated_reciprocal_w;
    }
}

void draw_textured_triangle(triangle_t triangle, uint32_t* texture) {
    int x0 = triangle.points[0].x;
    int y0 = triangle.points[0].y;
    float z0 = triangle.points[0].z;
    float w0 = triangle.points[0].w;
    float u0 = triangle.texcoords[0].u;
    float v0 = triangle.texcoords[0].v;
    int x1 = triangle.points[1].x;
    int y1 = triangle.points[1].y;
    float z1 = triangle.points[1].z;
    float w1 = triangle.points[1].w;
    float u1 = triangle.texcoords[1].u;
    float v1 = triangle.texcoords[1].v;
    int x2 = triangle.points[2].x;
    int y2 = triangle.points[2].y;
    float z2 = triangle.points[2].z;
    float w2 = triangle.points[2].w;
    float u2 = triangle.texcoords[2].u;
    float v2 = triangle.texcoords[2].v;

    // Sort Vertices to the following order (y0 < y1 < y2)
    if (y0 > y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&z0, &z1);
        float_swap(&w0, &w1);
        float_swap(&u0, &u1);
        float_swap(&v0, &v1);
    }
    if (y1 > y2) {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
        float_swap(&z1, &z2);
        float_swap(&w1, &w2);
        float_swap(&u1, &u2);
        float_swap(&v1, &v2);
    }
    if (y0 > y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&z0, &z1);
        float_swap(&w0, &w1);
        float_swap(&u0, &u1);
        float_swap(&v0, &v1);
    }

    // Flip the V component to account for the inverted UV-coordinates (V grows downwards).
    v0 = 1 - v0;
    v1 = 1 - v1;
    v2 = 1 - v2;

    vec4_t point_a = {x0, y0, z0, w0};
    vec4_t point_b = {x1, y1, z1, w1};
    vec4_t point_c = {x2, y2, z2, w2};
    tex2_t a_uv = {u0, v0};
    tex2_t b_uv = {u1, v1};
    tex2_t c_uv = {u2, v2};

    // Flat-Bottom Triangle
    float inverted_slope_1 = 0;
    float inverted_slope_2 = 0;

    if (y1 - y0 != 0) inverted_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
    if (y2 - y0 != 0) inverted_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

    if (y1 - y0 != 0) {
        for (int y = y0; y <= y1; y++) {
            int x_start = x1 + (y - y1) * inverted_slope_1;
            int x_end = x0 + (y - y0) * inverted_slope_2;

            if (x_end < x_start) int_swap(&x_start, &x_end);

            for (int x = x_start; x < x_end; x++) {
                draw_texture(x, y, point_a, point_b, point_c, a_uv, b_uv, c_uv, texture);
            }
        }
    }

    // Flat-Top Triangle
    if (y2 - y1 != 0) inverted_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
    if (y2 - y0 != 0) inverted_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

    if (y2 - y1 != 0) {
        for (int y = y1; y <= y2; y++) {
            int x_start = x1 + (y - y1) * inverted_slope_1;
            int x_end = x0 + (y - y0) * inverted_slope_2;

            if (x_end < x_start) int_swap(&x_start, &x_end);

            for (int x = x_start; x < x_end; x++) {
                draw_texture(x, y, point_a, point_b, point_c, a_uv, b_uv, c_uv, texture);
            }
        }
    }
}