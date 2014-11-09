#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#define TILES_MAX 4
#define TILES_SIZE 64

struct st_tile {
  SDL_Surface *tile;
};

void tiles_init();
void tiles_clean_up();
