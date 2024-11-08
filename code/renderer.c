// Need to include this so that SDL does not interfer with the main function in this file.
#define SDL_MAIN_HANDLED

#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

vec3_t camera_position = {.x=0, .y=0, .z=0};
float fov_factor = 640; // Magic Number for now!
bool is_running = false;
int previous_frame_time = 0;

da_array(mesh_triangles, triangle_t)
mesh_triangles triangles_to_render = {};

void setup() {
    // Colour Buffer is a 1D representation od a 2D array.
    colour_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
    assert(colour_buffer);

    render_settings = BACKFACE_CULLING | FILLED_TRIANGLES | WIREFRAME;

    // SDL Texture is used to display the colour buffer.
    colour_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
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
        } else if (event.key.keysym.sym == SDLK_1) {
            da_clear(mesh.vertices);
            da_clear(mesh.faces);
            load_obj_file_data("../assets/cube.obj");
        } else if (event.key.keysym.sym == SDLK_2) {
            da_clear(mesh.vertices);
            da_clear(mesh.faces);
            load_obj_file_data("../assets/f22.obj");
        }
        break;
    }
}

// Project a 3D vector in 2D.
vec2_t naive_orthographic_projection(vec3_t point) {
    vec2_t projected_point = {
        .x = (point.x * fov_factor) / point.z,
        .y = (point.y * fov_factor) / point.z
    };
    return projected_point;
}

void update(float x_rotation, float y_rotation, float z_rotation) {
    // Proper way of makeing the execution wait until the appropriate amount of time has passed.
    // Ensures that the OS is correctly interacted with instead of blocking everything with a while loop.
    // Issue with this approach is that greater FPS == faster rotation rather than smoother!
    // Think like how games ran during the 8/16bit era in the US vs UK.
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }
    mesh.rotation.x += x_rotation;
    mesh.rotation.y += y_rotation;
    mesh.rotation.z += z_rotation;

    int num_mesh_faces = mesh.faces.count;
    for (int i=0; i < num_mesh_faces; i++) {
        face_t mesh_face = mesh.faces.items[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices.items[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices.items[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices.items[mesh_face.c - 1];

        vec3_t transformed_vertices[3];

        for (int j=0; j < 3; j++) {
            vec3_t transformed_vertex = face_vertices[j];
            transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            transformed_vertex.z += 5;
            transformed_vertices[j] = transformed_vertex;
        }

        // Back Face Culling
        /*
            A
           / \
          C---B  */


        // vec3_t vector_a = transformed_vertices[0];
        // vec3_t vector_b = transformed_vertices[1];
        // vec3_t vector_c = transformed_vertices[2];

        // vec3_t vector_ab = vec3_subtract(vector_b, vector_a);
        // vec3_t vector_ac = vec3_subtract(vector_c, vector_a);

        // vec3_t normal = vec3_cross_product(vector_ab, vector_ac);
        // vec3_normalise(&normal);
        // vec3_t camera_ray = vec3_subtract(camera_position, vector_a);
        // float dot_product_camera = vec3_dot_product(normal, camera_ray);

        // // Only project the vertices which need to otherwise move onto the next face.
        // if (dot_product_camera < 0) {
        //     continue;
        // }

        triangle_t projected_triangle;
        for (int j=0; j < 3; j++) {
            vec2_t projected_vertex = naive_orthographic_projection(transformed_vertices[j]);
            projected_vertex.x += (window_width / 2);
            projected_vertex.y += (window_height / 2);

            projected_triangle.points[j] = projected_vertex;
        }

        if ((render_settings & BACKFACE_CULLING) == BACKFACE_CULLING) {
            vec2_t vertex_a = projected_triangle.points[0];
            vec2_t vertex_b = projected_triangle.points[1];
            vec2_t vertex_c = projected_triangle.points[2];

            float signed_2area = (vertex_a.x * vertex_b.y - vertex_b.x * vertex_a.y) + (vertex_b.x * vertex_c.y - vertex_c.x * vertex_b.y) + (vertex_c.x * vertex_a.y - vertex_a.x * vertex_c.y);
            if (signed_2area < 0) {
                da_append(&triangles_to_render, projected_triangle);
            } else continue;
        } else da_append(&triangles_to_render, projected_triangle);
    }
}

void render() {
    uint32_t yellow = 0xFFFFFF00;
    uint32_t black  = 0x00000000;
    uint32_t white  = 0xFFFFFFFF;
    uint32_t red    = 0xFFFF0000;
    uint32_t grey   = 0x00999999;
    uint32_t green  = 0xFF00FF00;
    uint32_t blue   = 0xFF0000FF;

    for (size_t triangle=0; triangle < triangles_to_render.count; triangle++) {
        if ((render_settings & (FILLED_TRIANGLES | WIREFRAME)) == (FILLED_TRIANGLES | WIREFRAME)) {
            draw_filled_triangle(triangles_to_render.items[triangle], grey);
            draw_triangle(triangles_to_render.items[triangle], white);
        }
        else if ((render_settings & FILLED_TRIANGLES) == FILLED_TRIANGLES) {
            draw_filled_triangle(triangles_to_render.items[triangle], grey);
        } else if ((render_settings & WIREFRAME) == WIREFRAME) {
            draw_triangle(triangles_to_render.items[triangle], white);
        }

        if ((render_settings & VERTEX_DOTS) == VERTEX_DOTS) {
            triangle_t current_triangle = triangles_to_render.items[triangle];
            draw_rectangle(current_triangle.points[0].x - 5, current_triangle.points[0].y - 5, 10, 10, red);
            draw_rectangle(current_triangle.points[0].x - 5, current_triangle.points[0].y - 5, 10, 10, red);
            draw_rectangle(current_triangle.points[1].x - 5, current_triangle.points[1].y - 5, 10, 10, red);
            draw_rectangle(current_triangle.points[1].x - 5, current_triangle.points[1].y - 5, 10, 10, red);
            draw_rectangle(current_triangle.points[2].x - 5, current_triangle.points[2].y - 5, 10, 10, red);
            draw_rectangle(current_triangle.points[2].x - 5, current_triangle.points[2].y - 5, 10, 10, red);
        } else {
            continue;
        }
    }

    da_clear(triangles_to_render);
    render_colour_buffer();
    clear_colour_buffer(black);
    SDL_RenderPresent(renderer);
}

void free_resources() {
  free(colour_buffer);
  da_free(&mesh.vertices);
  da_free(&mesh.faces);
}

int main(int argc, char *argv[]) {
    is_running = initialise_window();
    float x_rotation = 0.01;
    float y_rotation = 0.01;
    float z_rotation = 0.01;

    if (argc > 1) {
        printf("\n%s\n", argv[1]);
        if (strcmp(argv[1], "Cube") == 0) load_obj_file_data("../assets/cube.obj");
        else if (strcmp(argv[1], "F22") == 0) {
            load_obj_file_data("../assets/f22.obj");
            y_rotation = 0.0;
            z_rotation = 0.0;
        }
        else printf("Not a valid obj to load!");
    } else { load_obj_file_data("../assets/cube.obj"); }

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
