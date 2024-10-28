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
// JWG: Maybe update the function so that the offsetting is handled once in a seperate function?
void load_obj_file_data(char *filename) {
    FILE *stream;
    char line[100];

    printf("\nAbout to load obj file data!\n");
    if(fopen_s(&stream, filename, "r") == 0) {
        while (fgets(line, 100, stream) != NULL) {
            //Parse Vectors
            vec3_t current_vector;

            if ((line[0] == 'v') & (line[1] == ' ')) {
                int overall_offset = 0;

                int x_vector_start = 2;
                int x_vector_offset = 0;
                if (line[x_vector_start] == '-') {
                    overall_offset++;
                    x_vector_offset = 1;
                }
                char x_vector[10];
                for (int i = x_vector_start; i < x_vector_start + 8 + overall_offset; i++) {
                    x_vector[i-x_vector_start] = line[i];
                }
                x_vector[8+x_vector_offset] = '\0';

                int y_vector_start = 11 + overall_offset;
                int y_vector_offset = 0;
                if (line[y_vector_start] == '-') {
                    overall_offset++;
                    y_vector_offset= 1;
                }
                char y_vector[10];
                for (int i = y_vector_start; i < y_vector_start + 8 + overall_offset; i++) {
                    y_vector[i-y_vector_start] = line[i];
                }
                y_vector[8+y_vector_offset] = '\0';

                int z_vector_start = 20 + overall_offset;
                int z_vector_offset = 0;
                if (line[z_vector_start] == '-') {
                    overall_offset++;
                    z_vector_offset= 1;
                }
                char z_vector[10];
                for (int i = z_vector_start; i < z_vector_start + 8 + overall_offset; i++) {
                    z_vector[i-z_vector_start] = line[i];
                }
                z_vector[8+z_vector_offset] = '\0';

                vec3_t current_vector = {
                    .x = atof(x_vector),
                    .y = atof(y_vector),
                    .z = atof(z_vector)
                };

                da_append(&mesh.vertices, current_vector);
            }

            // Parse Faces
            if (line[0] == 'f') {
                char face_vertex_a[10];
                char face_vertex_b[10];
                char face_vertex_c[10];
                char face_vertices[100];
                char face = 'a';

                int line_length = strlen(line);
                const int VALUES_START_INDEX = 2;

                for (int i=2; i < line_length; i++) {
                    face_vertices[i-VALUES_START_INDEX] = line[i];
                }

                char* token = strtok(face_vertices, " ");
                while (token != NULL) {
                    char current_face_vertex[30];
                    int vertices_length = strlen(face_vertices);
                    int index = 0;

                    switch (face) {
                        case 'a':

                            while (index < vertices_length) {
                                current_face_vertex[index] = token[index];
                                index++;
                            }
                            current_face_vertex[index+1] = '\0';

                            strcpy(face_vertex_a, current_face_vertex);
                            face = 'b';
                            break;

                        case 'b':
                            while (index < vertices_length) {
                                current_face_vertex[index] = token[index];
                                index++;
                            }
                            current_face_vertex[index+1] = '\0';

                            strcpy(face_vertex_b, current_face_vertex);
                            face = 'c';
                            break;

                        case 'c':
                            while (index < vertices_length) {
                                current_face_vertex[index] = token[index];
                                index++;
                            }
                            current_face_vertex[index+1] = '\0';

                            strcpy(face_vertex_c, current_face_vertex);
                            face = 'a';
                            break;
                    }
                    token = strtok(NULL, " ");
                }

                face_t current_face = {
                    .a = atoi(face_vertex_a),
                    .b = atoi(face_vertex_b),
                    .c = atoi(face_vertex_c)
                };

                da_append(&mesh.faces, current_face);
            }
        }
    }
    fclose(stream);
}