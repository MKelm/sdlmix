#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "tiles.h"

#define TILES_MAX 4
#define TILES_SIZE 64

struct st_tile {
  SDL_Surface *tile;
} tiles[TILES_MAX];

SDL_Surface* tile_image;

char tile_names[TILES_MAX][128] = {
  "grass_64.png", "dirt_64.png", "water_64.png", "rock_64.png"
};

extern SDL_Surface *screen;

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

void tiles_load() {
  int i;
  for (i = 0; i < TILES_MAX; i++) {
    tiles[i].tile = tiles_load_img(tile_names[i]);
  }
}

void tiles_show() {
  int i;
  for (i = 0; i < TILES_MAX; i++) {
    SDL_Rect offset;
    offset.x = i * TILES_SIZE;
    offset.y = 0;

    SDL_BlitSurface(tiles[i].tile, NULL, screen, &offset);
  }
}

void tiles_clean_up() {
  int i;
  for (i = 0; i < TILES_MAX; i++) {
    SDL_FreeSurface(tiles[i].tile);
  }
}
