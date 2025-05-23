#ifndef DISPLAY_H
#define DISPLAY_H

#include "triangle.h"
#include <stdint.h>
#include <stdbool.h>
#include <sdl.h>

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* colour_buffer;
extern float* z_buffer;
extern SDL_Texture* colour_buffer_texture;
extern int window_width;
extern int window_height;

enum render_settings {
    FILLED_TRIANGLES    = 1 << 1,
    WIREFRAME           = 1 << 2,
    VERTEX_DOTS         = 1 << 3,
    BACKFACE_CULLING    = 1 << 4,
    TEXTURED            = 1 << 5
} render_settings;

bool initialise_window(void);
void my_draw_grid(int multiple);
void draw_grid(int multiple);
void draw_pixel(int x, int y, uint32_t colour);
void draw_rectangle(int x, int y, int width, int height, uint32_t colour);
void draw_line(int x0, int x1, int y0, int y1, uint32_t colour);
void draw_triangle(triangle_t triangle, uint32_t colour);
void render_colour_buffer(void);
void clear_colour_buffer(uint32_t colour);
void clear_z_buffer(void);
void destroy_window(void);

#endif
