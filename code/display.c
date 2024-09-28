#include "display.h"
#include <stdio.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL; 
uint32_t* colour_buffer = NULL;
SDL_Texture* colour_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

bool initialise_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initialising SDL!\n");
    return false;
  }

  // Set window to be fullscreen! Video mode is not true fullscreen yet though!
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  window_width = display_mode.w;
  window_height = display_mode.h;
  
  // Print out the actual window size and height! Then I don't get confused like I did when creating the rectangle! (JWG - 14/08/2024)  
  printf("Width: %i, Height: %i", window_width, window_height);
    
  // The OR'ing of the two flags means that true fullscreen is achieved.
  window = SDL_CreateWindow(
      "3D Graphics from Scratch", 
      SDL_WINDOWPOS_CENTERED, 
      SDL_WINDOWPOS_CENTERED, 
      window_width, 
      window_height,
      SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN
  );

  if (!window) {
      fprintf(stderr, "Error creating SDL Window!\n");
      return false;
  }
  
  // -1 means the default graphics driver is got.
  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer!\n");
    return false;
  }
  return true;
}

void my_draw_grid(int multiple) {
	    // Draw the grid's columns.
	    for (int h=0; h < window_height; h++) {
		        for (int w=0; w < window_width; w+=multiple) {
			            colour_buffer[(window_width * h) + w] = 0x00000000;    
			      }
		  }

	    // Draw the grid's rows.
	    for (int h=0; h < window_height; h+=multiple) {
		        for (int w=0; w < window_width; w++) {
			            colour_buffer[(window_width * h) + w] = 0x00000000;      
			      }
		  }    
}

void draw_grid(int multiple) {
	    for (int h=0; h < window_height; h++) {
		        for (int w=0; w < window_width; w++) {
			            if (w % multiple == 0 || h % multiple == 0) {
				                colour_buffer[(window_width * h) + w] = 0x00000000;
				          }
			      }
		    }
}


void draw_pixel(int x, int y, uint32_t colour) {
    if (x < window_width && y < window_height) {
        colour_buffer[(window_width * y) + x] = colour;
    }  
}
 
void draw_rectangle(int x, int y, int width, int height, uint32_t colour) {
    // x+width = side
    // y+height = bottom
    // loop through everything between x to w+width and y to y+height and assign colour

    int rect_side=-1; 
    int rect_bottom=-1;
    
    if (x+width <= window_width) {
        rect_side = x + width;
    } else {
        rect_side = window_width;
    }

    if (y+height <= window_height) {
        rect_bottom = y + height;    
    } else {
        rect_bottom = window_height;
    }
        
    for(int current_y=y; current_y <= rect_bottom; current_y++) {
        for (int current_x=x; current_x <= rect_side; current_x++) {
            draw_pixel(current_x, current_y, colour);      
        }
    }
}

void draw_line(int x0, int x1, int y0, int y1, uint32_t colour) {
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);
 
    int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

    float x_inc = delta_x / (float)side_length;
    float y_inc = delta_y / (float)side_length;

    float current_x = x0;
    float current_y = y0;
    for (int i=0; i < side_length; i++) {
        draw_pixel(round(current_x), round(current_y), colour);
        current_x += x_inc;
        current_y += y_inc;
    }
}

void draw_triangle(triangle_t triangle, uint32_t colour) {
    draw_line(triangle.points[0].x, triangle.points[1].x, triangle.points[0].y, triangle.points[1].y, colour);
    draw_line(triangle.points[1].x, triangle.points[2].x, triangle.points[1].y, triangle.points[2].y, colour);    
    draw_line(triangle.points[2].x, triangle.points[0].x, triangle.points[2].y, triangle.points[0].y, colour);   
}
 
void render_colour_buffer(void) {
    SDL_UpdateTexture(
          colour_buffer_texture
        , NULL
        , colour_buffer
        , (int)(window_width * sizeof(uint32_t))
    );
    SDL_RenderCopy(renderer, colour_buffer_texture, NULL, NULL);
}

void clear_colour_buffer(uint32_t colour) {
    for (int y=0; y < window_height; y++) {
        for (int x=0; x < window_width; x++) {
            colour_buffer[(window_width * y) + x] = colour; 
        }    
    }
}

void destroy_window(void) {
  free(colour_buffer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
