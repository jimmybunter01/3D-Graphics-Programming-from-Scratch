#include "texture.h"
#include "upng.h"

int texture_width;
int texture_height;
uint32_t* mesh_texture = NULL;
upng_t* png_texture = NULL;

void load_png_texture_data(char* filename) {
    png_texture = upng_new_from_file(filename);
  if (png_texture != NULL) {
    upng_decode(png_texture);
    if (upng_get_error(png_texture) == UPNG_EOK) {
        texture_width = upng_get_width(png_texture);
        texture_height = upng_get_height(png_texture);
        mesh_texture = (uint32_t *)upng_get_buffer(png_texture);
    }
  }
}

void free_textures() {
    upng_free(png_texture);
}