#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include "texture.h"
#include <stdint.h>

typedef struct {
	int a;
	int b;
	int c;

	// Needed for mapping textures correctly.
	tex2_t a_uv;
	tex2_t b_uv;
	tex2_t c_uv;

	uint32_t colour;
} face_t;

typedef struct {
	vec2_t points[3];
	tex2_t texcoords[3]; // Needed for mapping textures correctly.
	uint32_t colour;
	float avg_depth;
} triangle_t;

void draw_filled_triangle(triangle_t triangle, uint32_t colour);
void draw_textured_triangle(triangle_t triangle, uint32_t* texture);
void draw_texture(
    int x, int y
    , vec2_t point_a, vec2_t point_b, vec2_t point_c
    , float u0, float v0, float u1, float v1, float u2,  float v2
    , uint32_t* texture
);
#endif
