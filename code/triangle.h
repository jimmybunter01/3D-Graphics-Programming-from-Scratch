#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include <stdint.h>

typedef struct {
	int a;
	int b;
	int c;
} face_t;

typedef struct {
	vec2_t points[3];
} triangle_t;

void draw_filled_triangle(triangle_t triangle, uint32_t colour);

#endif
