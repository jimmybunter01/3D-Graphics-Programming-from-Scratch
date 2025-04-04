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

vec3_t cube_vertices[8] = {
    { .x = -1, .y = -1, .z = -1 }, // 1
    { .x = -1, .y =  1, .z = -1 }, // 2
    { .x =  1, .y =  1, .z = -1 }, // 3
    { .x =  1, .y = -1, .z = -1 }, // 4
    { .x =  1, .y =  1, .z =  1 }, // 5
    { .x =  1, .y = -1, .z =  1 }, // 6
    { .x = -1, .y =  1, .z =  1 }, // 7
    { .x = -1, .y = -1, .z =  1 }  // 8
};

face_t cube_faces[12] = {
    // front
    { .a = 1, .b = 2, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }},
    { .a = 1, .b = 3, .c = 4, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }},
    // right
    { .a = 4, .b = 3, .c = 5, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }},
    { .a = 4, .b = 5, .c = 6, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }},
    // back
    { .a = 6, .b = 5, .c = 7, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }},
    { .a = 6, .b = 7, .c = 8, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }},
    // left
    { .a = 8, .b = 7, .c = 2, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }},
    { .a = 8, .b = 2, .c = 1, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }},
    // top
    { .a = 2, .b = 7, .c = 5, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }},
    { .a = 2, .b = 5, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }},
    // bottom
    { .a = 6, .b = 8, .c = 1, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }},
    { .a = 6, .b = 1, .c = 4, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }}
};

void manual_cube_load(uint32_t colour) {
    for (int i=0; i < 8; i++) {
        da_append(&mesh.vertices, cube_vertices[i]);
    }

    for (int i=0; i < 12; i++) {
        face_t current_face = cube_faces[i];
        current_face.colour = colour;
        da_append(&mesh.faces, current_face);
    }
}

// This is a rudimentery parsing of a .obj file.
void load_obj_file_data(char *filename, uint32_t face_colours[], int no_of_colours) {
    FILE *stream;
    char line[100];

    // Start this at -1 as a side is indicated before the face vertices are specified in a .obj file.
    int colour_index = -1;
    printf("\nNo of Colours: %d", no_of_colours);

    printf("\nAbout to load obj file data!\n");
    if(fopen_s(&stream, filename, "r") == 0) {

        da_array(tex2_da_array, tex2_t)
        tex2_da_array tex_coords = {};

        while (fgets(line, 100, stream) != NULL) {
            //Parse Vectors
            if (strncmp(line, "v ", 2) == 0) {
                vec3_t current_vertex;
                sscanf(line, "v %f %f %f", &current_vertex.x, &current_vertex.y, &current_vertex.z);
                da_append(&mesh.vertices, current_vertex);
            }

            if (strncmp(line, "vt ", 3) == 0) {
                tex2_t tex_coord;
                sscanf(line, "vt %f %f", &tex_coord.u, &tex_coord.v);
                da_append(&tex_coords, tex_coord);
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
                    current_face.a = vertex_indices[0] - 1;
                    current_face.b = vertex_indices[1] - 1;
                    current_face.c = vertex_indices[2] - 1;

                    // In .obj file the indicies are NOT 0 based!
                    current_face.a_uv = tex_coords.items[texture_indices[0] - 1];
                    current_face.b_uv = tex_coords.items[texture_indices[1] - 1];
                    current_face.c_uv = tex_coords.items[texture_indices[2] - 1];
                    current_face.colour = face_colours[colour_index];
                }
                else {
                    current_face.a = vertex_indices[0] - 1;
                    current_face.b = vertex_indices[1] - 1;
                    current_face.c = vertex_indices[2] - 1;

                    // In .obj file the indicies are NOT 0 based!
                    current_face.a_uv = tex_coords.items[texture_indices[0] - 1];
                    current_face.b_uv = tex_coords.items[texture_indices[1] - 1];
                    current_face.c_uv = tex_coords.items[texture_indices[2] - 1];
                    
                    current_face.colour = face_colours[0];
                }
                da_append(&mesh.faces, current_face);
            }

            if (strncmp(line, "s ", 2) == 0) {
                colour_index++;
            }

        }
    da_clear(tex_coords);
    }
    fclose(stream);
}

