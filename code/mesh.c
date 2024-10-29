#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "array.h"
#include "mesh.h"

mesh_t mesh = {
  .vertices = NULL,
  .faces = NULL,
  .rotation = {0,0,0}
};

// This is a rudimentery parsing of a .obj file.
void load_obj_file_data(char *filename) {
    FILE *stream;
    char line[100];

    printf("\nAbout to load obj file data!\n");
    if(fopen_s(&stream, filename, "r") == 0) {
        while (fgets(line, 100, stream) != NULL) {
            //Parse Vectors
            if (strncmp(line, "v ", 2) == 0) {
                vec3_t current_vertex;
                sscanf(line, "v %f %f %f", &current_vertex.x, &current_vertex.y, &current_vertex.z);
                da_append(&mesh.vertices, current_vertex);
            }

            // Parse Faces
            if (strncmp(line, "f ", 2) == 0) {
                int vertex_indices[3];
                int texture_indices[3];
                int normal_indices[3];
                sscanf(
                    line
                    , "f %d/%d/%d %d/%d/%d %d/%d/%d"
                    , &vertex_indices[0], &texture_indices[0], &normal_indices[0]
                    , &vertex_indices[1], &texture_indices[1], &normal_indices[1]
                    , &vertex_indices[2], &texture_indices[2], &normal_indices[2]
                );

                face_t current_face = {
                    .a = vertex_indices[0],
                    .b = vertex_indices[1],
                    .c = vertex_indices[2]
                };
                da_append(&mesh.faces, current_face);
            }


        }
    }
    fclose(stream);
}