// Need to include this so that SDL does not interfer with the main function in this file.
#define SDL_MAIN_HANDLED

#include "display.h"
#include <stdbool.h>
#include <assert.h>

bool is_running = false;

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

void update() {
    // TODO
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    //draw_grid(10);
    draw_rectangle(300, 200, 300, 150, 0xFFFF00FF);
    render_colour_buffer();
    clear_colour_buffer(0xFFFFFF00);
    SDL_RenderPresent(renderer);
}

int main(void) {
  is_running = initialise_window();
  
  setup();

  while(is_running) {
      process_input();
      update();
      render();
  }

  destroy_window();
  
  return 0;
}

