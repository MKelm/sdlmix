#include "tiles.h"

struct st_tile tiles[TILES_MAX];

SDL_Surface* tile_image;

char tile_names[TILES_MAX][128] = {
  "grass_64.png", "dirt_64.png", "water_64.png", "rock_64.png"
};

SDL_Surface *tiles_load_img(char *file_name) {
  SDL_Surface* loaded_image = NULL;
  SDL_Surface* optimized_image = NULL;

  loaded_image = IMG_Load(file_name);
  if (loaded_image != NULL) {

    optimized_image = SDL_DisplayFormat(loaded_image);
    SDL_FreeSurface(loaded_image);
    if (optimized_image != NULL) {
      SDL_SetColorKey(
        optimized_image, SDL_SRCCOLORKEY, SDL_MapRGB(optimized_image->format, 0, 0, 0)
      );
    }
  }

  return optimized_image;
}

void tiles_init() {
  int i;
  for (i = 0; i < TILES_MAX; i++) {
    tiles[i].tile = tiles_load_img(tile_names[i]);
  }
}

void tiles_clean_up() {
  int i;
  for (i = 0; i < TILES_MAX; i++) {
    SDL_FreeSurface(tiles[i].tile);
  }
}
