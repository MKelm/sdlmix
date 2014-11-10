#include "map.h"
#include "tiles.h"

#define FALSE 0
#define TRUE 1

extern SDL_Surface *screen;
extern struct st_tile tiles[TILES_MAX];

int map[MAP_MAX_Y][MAP_MAX_X];
int map_loaded = FALSE;
int map_show_grid = FALSE;

SDL_Rect map_rect;
SDL_Rect map_move_rect;

void map_init() {
  tiles_init();

  map_rect.x = 0;
  map_rect.y = 0;
  map_move_reset();

  int row, col;
  for (row = 0; row < MAP_MAX_Y; row++) {
    for (col = 0; col < MAP_MAX_X; col++) {
      map[row][col] = -1;
    }
  }

  FILE *fp;
  if ((fp = fopen("map.dat", "r")) != NULL) {
    int row = 0, col = 0;
    char chunk[1024];
    char *chunk_part;
    while(fgets(chunk, 1024, fp) != NULL) {
      col = 0;
      chunk_part = strtok(chunk, " ");
      while (chunk_part != NULL) {
        if (col < MAP_MAX_X && row < MAP_MAX_Y) {
          map[row][col] = atoi(chunk_part);
        }
        chunk_part = strtok(NULL, " ");
        col++;
      }
      row++;
    }
    fclose(fp);
    map_loaded = TRUE;
  }
}

void map_toggle_grid() {
  map_show_grid = (map_show_grid == TRUE) ? FALSE : TRUE;
}

void map_show() {
  if (map_loaded == TRUE) {
    SDL_Rect offset;
    int row, col, x, y;
    y = map_rect.y;
    for (row = 0; row < MAP_MAX_Y; row++) {
      x = map_rect.x;
      for (col = 0; col < MAP_MAX_X; col++) {
        offset.x = x;
        offset.y = y;
        if (map[row][col] > -1) {
          SDL_BlitSurface(tiles[map[row][col]].tile, NULL, screen, &offset);
        }
        x += TILES_SIZE;
      }
      y += TILES_SIZE;
    }
  }

  if (map_show_grid == TRUE) {
    int row, col, x, y;
    y = map_rect.y;
    for (row = 0; row < MAP_MAX_Y; row++) {
      x = map_rect.x;
      lineRGBA(
        screen, x, y, x + MAP_MAX_X * TILES_SIZE, y,
        255, 255, 255, 255
      );
      for (col = 0; col < MAP_MAX_X; col++) {
        lineRGBA(
          screen, x, y, x, y + MAP_MAX_Y * TILES_SIZE - row * TILES_SIZE,
          255, 255, 255, 255
        );
        x += TILES_SIZE;
      }
      lineRGBA(
        screen, x, y, x, y + MAP_MAX_Y * TILES_SIZE - row * TILES_SIZE,
        255, 255, 255, 255
      );
      y += TILES_SIZE;
    }
    x = map_rect.x;
    lineRGBA(
      screen, x, y, x + MAP_MAX_X * TILES_SIZE, y,
      255, 255, 255, 255
    );
  }
}

void map_move(int x, int y) {
  if (map_move_rect.x > -1 && map_move_rect.y > -1) {
    map_rect.x -= map_move_rect.x - x;
    map_rect.y -= map_move_rect.y - y;
  }
  map_move_rect.x = x;
  map_move_rect.y = y;
}

void map_move_reset() {
  map_move_rect.x = -1;
  map_move_rect.y = -1;
}

void map_clean_up() {
  tiles_clean_up();
}
