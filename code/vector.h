#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x,y;
} vec2_t;

typedef struct {
    float x, y, z;
} vec3_t;


// 2D Vector Functions
float vec2_length(vec2_t vector);
vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_subtract(vec2_t a, vec2_t b);
vec2_t vec2_multiply(vec2_t vector, float scalar);
vec2_t vec2_divide(vec2_t vector, float scalar);
float vec2_dot_product(vec2_t a, vec2_t b);
void vec2_normalise(vec2_t *vector);

// 3D Vector Functions
float vec3_length(vec3_t vector);
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_subtract(vec3_t a, vec3_t b);
vec3_t vec3_multiply(vec3_t vector, float scalar);
vec3_t vec3_divide(vec3_t vector, float scalar);
vec3_t vec3_cross_product(vec3_t a, vec3_t b);
float vec3_dot_product(vec3_t a, vec3_t b);
void vec3_normalise(vec3_t *vector);

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);

#endif
