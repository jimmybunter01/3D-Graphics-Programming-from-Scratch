// Need to include this so that SDL does not interfer with the main function in this file.
#define SDL_MAIN_HANDLED

#include "display.h"
#include "vector.h"
#include "colours.h"
#include "mesh.h"
#include "array.h"
#include "triangle.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

vec3_t camera_position = {.x=0, .y=0, .z=0};
bool is_running = false;
int previous_frame_time = 0;
float x_rotation;
float y_rotation;
float z_rotation;
da_array(mesh_triangles, triangle_t)
mesh_triangles triangles_to_render = {};
mat4_t perspective_projection_matrix;

void load_cube_data() {
    uint32_t face_colours[] = {YELLOW, GREEN, BLUE, RED, GREY, WHITE};
    // uint32_t face_colours[] = {WHITE};
    int no_of_colours = sizeof(face_colours) / sizeof(face_colours[0]);
    x_rotation = y_rotation = z_rotation = 0.001;
    mesh.scale.x = mesh.scale.y = mesh.scale.z = 1;
    load_obj_file_data("../assets/cube.obj", face_colours, no_of_colours);
    load_png_texture_data("../assets/cube.png");
}

void load_f22_data() {
    uint32_t face_colours[1] = {WHITE};
    y_rotation = z_rotation = 0.01;
    mesh.rotation.x = mesh.rotation.y = mesh.rotation.z = 0;
    mesh.scale.x = mesh.scale.y = mesh.scale.z = 1;
    load_obj_file_data("../assets/f22.obj", face_colours, 1);
    load_png_texture_data("../assets/f22.png");
}

void load_f117_data() {
    uint32_t face_colours[1] = {WHITE};
    y_rotation = z_rotation = 0.01;
    mesh.rotation.x = mesh.rotation.y = mesh.rotation.z = 0;
    mesh.scale.x = mesh.scale.y = mesh.scale.z = 1;
    load_obj_file_data("../assets/f117.obj", face_colours, 1);
    load_png_texture_data("../assets/f117.png");
}

void load_efa_data() {
    uint32_t face_colours[1] = {WHITE};
    y_rotation = z_rotation = 0.01;
    mesh.rotation.x = mesh.rotation.y = mesh.rotation.z = 0;
    mesh.scale.x = mesh.scale.y = mesh.scale.z = 1;
    load_obj_file_data("../assets/efa.obj", face_colours, 1);
    load_png_texture_data("../assets/efa.png");
}

void load_drone_data() {
    uint32_t face_colours[1] = {WHITE};
    y_rotation = z_rotation = 0.01;
    mesh.rotation.x = mesh.rotation.y = mesh.rotation.z = 0;
    mesh.scale.x = mesh.scale.y = mesh.scale.z = 1;
    load_obj_file_data("../assets/drone.obj", face_colours, 1);
    load_png_texture_data("../assets/drone.png");
}

void load_crab_data() {
    uint32_t face_colours[1] = {WHITE};
    y_rotation = z_rotation = 0.01;
    mesh.rotation.x = mesh.rotation.y = mesh.rotation.z = 0;
    mesh.scale.x = mesh.scale.y = mesh.scale.z = 1;
    load_obj_file_data("../assets/crab.obj", face_colours, 1);
    load_png_texture_data("../assets/crab.png");
}

void load_sphere_data() {
    uint32_t face_colours[1] = {WHITE};
    y_rotation = z_rotation = 0.01;
    mesh.rotation.x = mesh.rotation.y = mesh.rotation.z = 0;
    mesh.scale.x = mesh.scale.y = mesh.scale.z = 1;
    load_obj_file_data("../assets/sphere.obj", face_colours, 1);
    load_png_texture_data("../assets/pikuma.png");
}

void setup() {
    // Colour Buffer is a 1D representation od a 2D array.
    colour_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
    z_buffer = (float*) malloc(sizeof(float) * window_width * window_height);

    render_settings = BACKFACE_CULLING | TEXTURED;

    float fov = M_PI / 1.75; // FOV is given in Radians.
    float aspect = (float)window_height/ (float)window_width;
    float znear = 0.1; // Arbitraary Value
    float zfar = 100.0; // Arbitraary Value
    perspective_projection_matrix = mat4_make_perspective(fov, aspect, zfar, znear);


    // SDL Texture is used to display the colour buffer.
    colour_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );
}

void process_input() {
    SDL_Event event;
    SDL_PollEvent(&event);

    // SDL_Quit event is the vent which is sent to the OS when you press `X` button to close a window.
    switch(event.type) {
      case SDL_QUIT:
        is_running = false;
        break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            is_running = false;
        } else if (event.key.keysym.sym == SDLK_w) {
            render_settings ^= WIREFRAME;
        } else if (event.key.keysym.sym == SDLK_d) {
            render_settings ^= VERTEX_DOTS;
        } else if (event.key.keysym.sym == SDLK_f) {
            render_settings ^= FILLED_TRIANGLES;
        } else if (event.key.keysym.sym == SDLK_c) {
            render_settings ^= BACKFACE_CULLING;
        } else if (event.key.keysym.sym == SDLK_t) {
            render_settings ^= TEXTURED;
        } else if (event.key.keysym.sym == SDLK_1) {
            da_clear(mesh.vertices);
            da_clear(mesh.faces);
            load_cube_data();
        } else if (event.key.keysym.sym == SDLK_2) {
            da_clear(mesh.vertices);
            da_clear(mesh.faces);
            load_f22_data();
        } else if (event.key.keysym.sym == SDLK_3) {
            da_clear(mesh.vertices);
            da_clear(mesh.faces);
            load_f117_data();
        } else if (event.key.keysym.sym == SDLK_4) {
            da_clear(mesh.vertices);
            da_clear(mesh.faces);
            load_efa_data();
        } else if (event.key.keysym.sym == SDLK_5) {
            da_clear(mesh.vertices);
            da_clear(mesh.faces);
            load_drone_data();
        } else if (event.key.keysym.sym == SDLK_6) {
            da_clear(mesh.vertices);
            da_clear(mesh.faces);
            load_crab_data();
        } else if (event.key.keysym.sym == SDLK_7) {
            da_clear(mesh.vertices);
            da_clear(mesh.faces);
            load_sphere_data();
        }
        break;
    }
}

// Project a 3D vector in 2D.
// vec2_t naive_orthographic_projection(vec3_t point) {
//     vec2_t projected_point = {
//         .x = (point.x * fov_factor) / point.z,
//         .y = (point.y * fov_factor) / point.z
//     };
//     return projected_point;
// }

// Sorting for painters algorithm - not needed with a Z-Buffer.
// int mesh_triangle_quicksort_partition(triangle_t *triangles, int start, int end) {
//     float pivot = triangles[end].avg_depth;
//     int i = start;
//     for (int j=start; j < end; j++) {
//         if (triangles[j].avg_depth >= pivot) {
//             triangle_swap(&triangles[i], &triangles[j]);
//             i++;
//         }
//     }
//     triangle_swap(&triangles[i], &triangles[end]);
//     return i;
// }

// void mesh_triangle_quicksort(triangle_t *triangles_to_sort, int start, int end) {
//     /*
//     Lomuto Partition Scheme https://www.wikiwand.com/en/articles/Quicksort#Lomuto_partition_scheme

//     ---Steps:---
//     1. If count < 2 do nothing.
//     2. Pick a pivot point.
//     3. Split the array based on this pivot point.
//         i. All the value less than this must be before the pivot.
//         ii. Apply recursively.
//     */

//     if ((start >= end) || (start < 0)) {
//         return;
//     } else {
//         int pivot_index = mesh_triangle_quicksort_partition(triangles_to_sort, start, end);
//         mesh_triangle_quicksort(triangles_to_sort, start, (pivot_index - 1));
//         mesh_triangle_quicksort(triangles_to_sort, pivot_index + 1, end);
//     }
// }

void update(float x_rotation, float y_rotation, float z_rotation) {
    // Proper way of makeing the execution wait until the appropriate amount of time has passed.
    // Ensures that the OS is correctly interacted with instead of blocking everything with a while loop.
    // Issue with this approach is that greater FPS == faster rotation rather than smoother!
    // Think like how games ran during the 8/16bit era in the US vs UK.
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    mesh.rotation.x += 0.01;
    mesh.rotation.y += 0;
    mesh.rotation.z += 0;
    // mesh.scale.x += 0.002;
    // mesh.scale.y += 0.001;
    // mesh.translation.x += 0.01;
    mesh.translation.z = 5.0;

    mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
    mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
    mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
    mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
    mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);

    mat4_t world_matrix = mat4_identity();
    world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
    world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

    int num_mesh_faces = mesh.faces.count;
    for (int i=0; i < num_mesh_faces; i++) {
        face_t mesh_face = mesh.faces.items[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices.items[mesh_face.a];
        face_vertices[1] = mesh.vertices.items[mesh_face.b];
        face_vertices[2] = mesh.vertices.items[mesh_face.c];

        vec4_t transformed_vertices[3];

        for (int j=0; j < 3; j++) {
            vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

            // Matrix replaces this code!
            // transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            // transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            // transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            // World Matrix Replaces this code!
            // Need to make sure that the oder of operations is correct - Matrix Multiplcation is not commutative.
            // transformed_vertex = mat4_mul_vec4(scale_matrix, transformed_vertex);
            // transformed_vertex = mat4_mul_vec4(rotation_matrix_x, transformed_vertex);
            // transformed_vertex = mat4_mul_vec4(rotation_matrix_y, transformed_vertex);
            // transformed_vertex = mat4_mul_vec4(rotation_matrix_z, transformed_vertex);
            // transformed_vertex = mat4_mul_vec4(translation_matrix, transformed_vertex);

            transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);
            transformed_vertices[j] = transformed_vertex;
        }

        // Back Face Culling
        /*
            A
           / \
          C---B  */

        // if ((render_settings & BACKFACE_CULLING) == BACKFACE_CULLING) {
        //     vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]);
        //     vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]);
        //     vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]);

        //     vec3_t vector_ab = vec3_subtract(vector_b, vector_a);
        //     vec3_t vector_ac = vec3_subtract(vector_c, vector_a);
        //     vec3_normalise(&vector_ab);
        //     vec3_normalise(&vector_ac);


        //     vec3_t normal = vec3_cross_product(vector_ab, vector_ac);
        //     vec3_normalise(&normal);

        //     vec3_t camera_ray = vec3_subtract(camera_position, vector_a);
        //     float dot_product_camera = vec3_dot_product(normal, camera_ray);

        //     // Only project the vertices which need to otherwise move onto the next face.
        //     if (dot_product_camera < 0) {
        //         continue;
        //     }
        // }

        vec4_t projected_points[3];
        for (int j=0; j < 3; j++) {
            // projected_points[j] = naive_orthographic_projection(vec3_from_vec4(transformed_vertices[j]));
            projected_points[j] = mat4_mul_vec4_project(perspective_projection_matrix, transformed_vertices[j]);

            // Scale into the view
            projected_points[j].x *= (window_width>>1);
            projected_points[j].y *= (window_height>>1);

            // Invert the y values to account for flipped screen y coordinates
            // projected_points[j].y *= -1;

            // Translate the points to the middle.
            projected_points[j].x += (window_width>>1);
            projected_points[j].y += (window_height>>1);
        }

        vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]);
        vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]);
        vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]);

        vec3_t vector_ab = vec3_subtract(vector_a, vector_b);
        vec3_t vector_ac = vec3_subtract(vector_a, vector_c);
        vec3_t normal = vec3_cross_product(vector_ab, vector_ac);
        vec3_normalise(&normal);

        // Negative to ensure that we are calculating the correct intensinty factor.
        // Intuitive to envision the 'photons' of light we've placed heading along the direction of light, which makes sense.
        // Instead we need the direction of the ray we need to use to get the correct dot product value has to be inverse.
        float light_intensity_factor = -vec3_dot_product(normal, light.direction);
        uint32_t new_colour = light_apply_intensity(mesh_face.colour, light_intensity_factor);

        triangle_t projected_triangle = {
            .points = {
                {projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w},
                {projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w},
                {projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w}
            },
            . texcoords = {
                {mesh_face.a_uv.u, mesh_face.a_uv.v},
                {mesh_face.b_uv.u, mesh_face.b_uv.v},
                {mesh_face.c_uv.u, mesh_face.c_uv.v}
            },
            .colour = new_colour,
            .avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3
        };

        if ((render_settings & BACKFACE_CULLING) == BACKFACE_CULLING) {
            vec2_t vertex_a = vec2_from_vec4(projected_triangle.points[0]);
            vec2_t vertex_b = vec2_from_vec4(projected_triangle.points[1]);
            vec2_t vertex_c = vec2_from_vec4(projected_triangle.points[2]);

            // Negative signs are a fix to make signed area code work with projected points which have been inverted!
            float signed_2area = (vertex_a.x * vertex_b.y - vertex_b.x * vertex_a.y) + (vertex_b.x * vertex_c.y - vertex_c.x * vertex_b.y) + (vertex_c.x * vertex_a.y - vertex_a.x * vertex_c.y);
            if (signed_2area < 0) {
                da_append(&triangles_to_render, projected_triangle);
            } else continue;
        } else da_append(&triangles_to_render, projected_triangle);
        da_append(&triangles_to_render, projected_triangle);
    }
    // mesh_triangle_quicksort(triangles_to_render.items, 0, triangles_to_render.count-1);
}

void render() {
    for (size_t i=0; i < triangles_to_render.count; i++) {
        triangle_t triangle = triangles_to_render.items[i];

        // Seperate if statements for all of these mean we can just turn the flags true/false (on/off) without having to think about various combinations!
        if ((render_settings & FILLED_TRIANGLES) == FILLED_TRIANGLES) {
            draw_filled_triangle(triangle, triangle.colour);
        }

        if ((render_settings & TEXTURED) == TEXTURED) {
            draw_textured_triangle(triangle, mesh_texture);
        }

        if ((render_settings & WIREFRAME) == WIREFRAME) {
            draw_triangle(triangle, WHITE);
        }

        if ((render_settings & VERTEX_DOTS) == VERTEX_DOTS) {
            draw_rectangle(triangle.points[0].x - 5, triangle.points[0].y - 5, 10, 10, RED);
            draw_rectangle(triangle.points[0].x - 5, triangle.points[0].y - 5, 10, 10, RED);
            draw_rectangle(triangle.points[1].x - 5, triangle.points[1].y - 5, 10, 10, RED);
            draw_rectangle(triangle.points[1].x - 5, triangle.points[1].y - 5, 10, 10, RED);
            draw_rectangle(triangle.points[2].x - 5, triangle.points[2].y - 5, 10, 10, RED);
            draw_rectangle(triangle.points[2].x - 5, triangle.points[2].y - 5, 10, 10, RED);
        }
    }

    da_clear(triangles_to_render);
    render_colour_buffer();
    clear_colour_buffer(BLACK);
    clear_z_buffer();
    SDL_RenderPresent(renderer);
}

void free_resources() {
  free(colour_buffer);
  da_free(&mesh.vertices);
  da_free(&mesh.faces);
  free_textures();
}

int main(int argc, char *argv[]) {
    is_running = initialise_window();

    if (argc > 1) {
        printf("\n%s\n", argv[1]);
        if (strcmp(argv[1], "Cube") == 0) load_cube_data();
        else if (strcmp(argv[1], "F22") == 0) load_f117_data();
        else printf("Not a valid obj to load!");
    } else load_f117_data();

    setup();

    while (is_running) {
        process_input();
        update(x_rotation, y_rotation, z_rotation);
        render();
    }

    destroy_window();
    free_resources();
    return 0;
}
