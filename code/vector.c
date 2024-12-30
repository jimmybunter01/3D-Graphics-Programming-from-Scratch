#include "vector.h"
#include <math.h>

float vec2_length(vec2_t vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

vec2_t vec2_add(vec2_t a, vec2_t b) {
    vec2_t new_vector = {
        .x = a.x + b.x,
        .y = a.y + b.y
    };
    return new_vector;
}

vec2_t vec2_subtract(vec2_t a, vec2_t b) {
    vec2_t new_vector = {
        .x = a.x - b.x,
        .y = a.y - b.y
    };
    return new_vector;
}

vec2_t vec2_multiply(vec2_t vector, float scalar) {
    vec2_t new_vector = {
        .x = vector.x * scalar,
        .y = vector.y * scalar
    };
    return new_vector;
}

vec2_t vec2_divide(vec2_t vector, float scalar) {
    vec2_t new_vector = {
        .x = vector.x / scalar,
        .y = vector.y / scalar
    };
    return new_vector;
}

float vec2_dot_product(vec2_t a, vec2_t b) {
    return (a.x * b.x) + (a.y * b.y);
}

void vec2_normalise(vec2_t *vector) {
    float length = sqrt(vector->x * vector->x + vector->y * vector->y);

    vector->x /= length;
    vector->y /= length;
}

float vec3_length(vec3_t vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
    vec3_t new_vector = {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z
    };
    return new_vector;
}

vec3_t vec3_subtract(vec3_t a, vec3_t b) {
    vec3_t new_vector = {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z
    };
    return new_vector;
}

vec3_t vec3_multiply(vec3_t vector, float scalar) {
    vec3_t new_vector = {
        .x = vector.x * scalar,
        .y = vector.y * scalar,
        .z = vector.z * scalar
    };
    return new_vector;
}

vec3_t vec3_divide(vec3_t vector, float scalar) {
    vec3_t new_vector = {
        .x = vector.x / scalar,
        .y = vector.y / scalar,
        .z = vector.z / scalar
    };
    return new_vector;
}

vec3_t vec3_cross_product(vec3_t a, vec3_t b) {
    vec3_t normal_vector = {
        .x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x
    };
    return normal_vector;
}

float vec3_dot_product(vec3_t a, vec3_t b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void vec3_normalise(vec3_t *vector) {
    float length = sqrt(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
    vector->x /= length;
    vector->y /= length;
    vector->z /= length;
}

vec3_t vec3_rotate_x(vec3_t v, float angle) {
  vec3_t rotated_vector = {
      .x = v.x,
      .y = v.y * cos(angle) - v.z * sin(angle),
      .z = v.z * cos(angle) + v.y * sin(angle),
  };
  return rotated_vector;
}


vec3_t vec3_rotate_y(vec3_t v, float angle) {
  vec3_t rotated_vector = {
      .x = v.x * cos(angle) + v.z * sin(angle),
      .y = v.y,
      .z = v.z * cos(angle) + v.x * sin(angle)
  };
  return rotated_vector;
}


vec3_t vec3_rotate_z(vec3_t v, float angle) {
  vec3_t rotated_vector = {
      .x = v.x * cos(angle) - v.y * sin(angle),
      .y = v.y * cos(angle) + v.x * sin(angle),
      .z = v.z
  };
  return rotated_vector;
}

vec4_t vec4_from_vec3(vec3_t v) {
    vec4_t result = {v.x, v.y, v.z, 1.0};
    return result;
}

vec3_t vec3_from_vec4(vec4_t v) {
    vec3_t result = {v.x, v.y, v.z};
    return result;
}