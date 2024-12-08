#include "array.h"
#include "mesh.h"
#include "colours.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

mesh_t mesh = {
  .vertices = NULL,
  .faces = NULL,
  .rotation = {0,0,0},
  .scale = {1.0,1.0,1.0},
  .translation = {0,0,0}
};

// This is a rudimentery parsing of a .obj file.
void load_obj_file_data(char *filename, uint32_t face_colours[], int no_of_colours) {
    FILE *stream;
    char line[100];

    // Start this at -1 as a side is indicated before the face vertices are specified in a .obj file.
    int colour_index = -1;
    printf("\nNo of Colours: %d", no_of_colours);

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

                face_t current_face = {};
                if (no_of_colours > 1) {
                    current_face.a = vertex_indices[0];
                    current_face.b = vertex_indices[1];
                    current_face.c = vertex_indices[2];
                    current_face.colour = face_colours[colour_index];
                }
                else {
                    current_face.a = vertex_indices[0];
                    current_face.b = vertex_indices[1];
                    current_face.c = vertex_indices[2];
                    current_face.colour = face_colours[0];
                }
                da_append(&mesh.faces, current_face);
            }

            if (strncmp(line, "s ", 2) == 0) {
                colour_index++;
            }
        }
    }
    fclose(stream);
}

