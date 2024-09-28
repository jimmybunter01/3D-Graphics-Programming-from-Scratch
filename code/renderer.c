// Need to include this so that SDL does not interfer with the main function in this file.
#define SDL_MAIN_HANDLED

#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

da_array(triangle_mesh, triangle_t);
triangle_mesh triangles_to_render = {};

vec3_t camera_position = {.x=0, .y=0, .z=-5};
vec3_t cube_rotation = {.x=0, .y=0, .z=0};

float fov_factor = 640; // Magic Number for now!
bool is_running = false;
int previous_frame_time = 0;

void setup() {
    // Colour Buffer is a 1D representation od a 2D array.
    colour_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
    assert(colour_buffer);
    
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

void update() {   
    // Proper way of makeing the execution wait until the appropriate amount of time has passed.
    // Ensures that the OS is correctly interacted with instead of blocking everything with a while loop.
    // Issue with this approach is that greater FPS == faster rotation rather than smoother!
    // Think like how games ran during the 8/16bit era in the US vs UK.
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }
    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    for (int i=0; i < N_MESH_FACES; i++) {
        face_t mesh_face = mesh_faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh_vertices[mesh_face.a - 1];
        face_vertices[1] = mesh_vertices[mesh_face.b - 1];
        face_vertices[2] = mesh_vertices[mesh_face.c - 1];

        triangle_t projected_triangle;
        for (int j=0; j < 3; j++) {
            vec3_t transformed_vertex = face_vertices[j];
            transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y); 
            transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);    

            transformed_vertex.z -= camera_position.z;
        
            vec2_t projected_vertex = naive_orthographic_projection(transformed_vertex);
            projected_vertex.x += (window_width / 2);
            projected_vertex.y += (window_height / 2);
            
            projected_triangle.points[j] = projected_vertex;
        }
        
        da_append(&triangles_to_render, projected_triangle);
    }
}

void render() {
    uint32_t yellow = 0xFFFFFF00;
    uint32_t black = 0x00000000;
    int TRIANGLE_VERTICIES = 3;
        
    for (size_t triangle=0; triangle < triangles_to_render.count; triangle++) {
        for (int vertex=0; vertex<TRIANGLE_VERTICIES; vertex++) {
             draw_rectangle(triangles_to_render.items[triangle].points[vertex].x, triangles_to_render.items[triangle].points[vertex].y, 3, 3, yellow); 
        }
        draw_triangle(triangles_to_render.items[triangle], yellow);
    } 

    da_clear(triangles_to_render);           
    render_colour_buffer();
    clear_colour_buffer(black);    
    SDL_RenderPresent(renderer);
}

int main(void) {
  is_running = initialise_window();
  setup();
  while (is_running) {
      process_input();
      update();
      render();
  }
  destroy_window();
  return 0;
}
