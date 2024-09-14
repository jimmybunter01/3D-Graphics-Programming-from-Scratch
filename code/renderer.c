// Need to include this so that SDL does not interfer with the main function in this file.
#define SDL_MAIN_HANDLED

// TL;DR this is done because a variable for an array size suggests a variable length array (VLA) which is not supported by the MSVC compiler.
// The value of the variable is only known as runtime rather than compile time.
// Could do #define NO_POINTS(x) (x*x*x) to  
#define NO_POINTS 729

#include "display.h"
#include "vector.h"
#include <stdbool.h>
#include <assert.h>

vec3_t cube_points[NO_POINTS];
vec2_t projected_points[NO_POINTS];
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
  
    int point_count = 0;
    for (float x=-1; x<=1; x+=0.25) {
         for (float y=-1; y<=1; y+=0.25) {
             for (float z=-1; z<=1; z+=0.25) {
                 vec3_t new_point = {.x = x, .y = y, .z = z};
                 cube_points[point_count++] = new_point;
             }
         }  
    }
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
    // SDL Init causes SDL to start coutning how many seconds have passed.
    // Execution is locked unti the correct amount of time has passed - should keep the FPS constant across all devices this code is executed on.
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME));
    previous_frame_time = SDL_GetTicks();

    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;
    
    for (int i=0; i < NO_POINTS; i++) {
        vec3_t point = cube_points[i];

        vec3_t transformed_point = vec3_rotate_x(point, cube_rotation.x);    
        transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
        transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);
        
        // Translate points away from the camera
        transformed_point.z -= camera_position.z;
        
        vec2_t projected_point = naive_orthographic_projection(transformed_point);
        projected_points[i] = projected_point;
    }
}

void render() {
    //draw_grid(10);
    
    //draw_pixel(50, 50, 0xFFFF00FF);
    for (int i=0; i < NO_POINTS; i++) {
        vec2_t projected_point = projected_points[i];
        draw_rectangle(projected_point.x + (window_width / 2), projected_point.y + (window_height / 2), 4, 4, 0xFFFFFF00);
    }
    
    render_colour_buffer();
    clear_colour_buffer(0x00000000);    
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
