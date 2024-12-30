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

#endif
