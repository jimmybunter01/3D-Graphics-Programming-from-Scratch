#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"
#include "array.h"
#include <stdlib.h>

da_array(verticies_t, vec3_t);
da_array(faces_t, face_t);
da_array(triangles, triangle_t);

typedef struct {
  verticies_t vertices;
  faces_t faces;
  vec3_t rotation;
  vec3_t scale;
  vec3_t translation;
} mesh_t;

extern mesh_t mesh;

void load_obj_file_data(char* filename, uint32_t face_colours[], int no_of_colours);

#endif
