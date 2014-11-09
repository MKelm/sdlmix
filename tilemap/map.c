#include "map.h"
#include "tiles.h"

extern SDL_Surface *screen;
extern struct st_tile tiles[TILES_MAX];

int map[MAP_MAX_Y][MAP_MAX_X];

void map_init() {
  tiles_init();

  FILE *fp;
  if ((fp = fopen("map.dat", "r")) == NULL) {
    fprintf(stderr, "Error while loading map data.\n");
    exit(EXIT_FAILURE);
  }

  int row = 0, col = 0;
  char chunk[1024];
  char *chunk_part;
  while(fgets(chunk, 1024, fp) != NULL) {
    col = 0;
    chunk_part = strtok(chunk, " ");
    while (chunk_part != NULL) {
      map[row][col] = atoi(chunk_part);
      chunk_part = strtok(NULL, " ");
      col++;
    }
    row++;
  }
  fclose(fp);

}

void map_show() {
  SDL_Rect offset;
  int row, col, x, y;
  y = 0;
  for (row = 0; row < MAP_MAX_Y; row++) {
    x = 0;
    for (col = 0; col < MAP_MAX_X; col++) {
      offset.x = x;
      offset.y = y;
      SDL_BlitSurface(tiles[map[row][col]].tile, NULL, screen, &offset);
      x += TILES_SIZE;
    }
    y += TILES_SIZE;
  }
}

void map_clean_up() {
  tiles_clean_up();
}
