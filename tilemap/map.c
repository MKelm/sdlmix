#include "map.h"
#include "tiles.h"

extern SDL_Surface *screen;
extern struct st_tile tiles[TILES_MAX];

void map_init() {
  tiles_init();
}

void map_show() {
  int i;
  for (i = 0; i < TILES_MAX; i++) {
    SDL_Rect offset;
    offset.x = i * TILES_SIZE;
    offset.y = 0;

    SDL_BlitSurface(tiles[i].tile, NULL, screen, &offset);
  }
}

void map_clean_up() {
  tiles_clean_up();
}
