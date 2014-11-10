#include "map.h"
#include "tiles.h"

#define FALSE 0
#define TRUE 1

extern SDL_Surface *screen;
extern struct st_tile tiles[TILES_MAX];

int map_rows = 4;
int map_cols = 4;
int **map;
int map_set = FALSE;
int map_loaded = FALSE;
int map_show_grid = TRUE;

struct st_map_tile_selection {
  int row;
  int col;
} map_tile_selection;

SDL_Rect map_rect;
SDL_Rect map_move_rect;

void map_set_map() {
  if (map_set == TRUE) {
    free(map);
  }

  map_rect.x = 0;
  map_rect.y = 0;
  map_rect.w = map_cols * TILES_SIZE;
  map_rect.h = map_rows * TILES_SIZE;
  map_tile_selection_reset();

  int i;
  map = (int **) malloc(map_rows * sizeof(int *));
  for (i = 0; i < map_rows; i++)
    map[i] = (int *) malloc(map_cols * sizeof(int));

  int row, col;
  for (row = 0; row < map_rows; row++) {
    for (col = 0; col < map_cols; col++) {
      map[row][col] = -1;
    }
  }

  map_set = TRUE;
}

void map_init() {
  tiles_init();

  map_set_map();
  map_move_reset();
}

void map_load() {
  FILE *fp;
  if ((fp = fopen("map.dat", "r")) != NULL) {
    int row = 0, col = 0;
    char chunk[1024];
    char *chunk_part;
    while(fgets(chunk, 1024, fp) != NULL) {
      col = 0;
      chunk_part = strtok(chunk, " ");
      while (chunk_part != NULL) {
        if (col < map_cols && row < map_rows) {
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

void map_resize(int direction) {
  if (direction == 1) {
    map_rows++;
    map_cols++;
  } else if (map_rows > 1 && map_cols > 1) {
    map_rows--;
    map_cols--;
  }
  map_set_map();
}

void map_toggle_grid() {
  map_show_grid = (map_show_grid == TRUE) ? FALSE : TRUE;
}

void map_show() {
  if (map_loaded == TRUE) {
    SDL_Rect offset;
    int row, col, x, y;
    y = map_rect.y;
    for (row = 0; row < map_rows; row++) {
      x = map_rect.x;
      for (col = 0; col < map_cols; col++) {
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
    for (row = 0; row < map_rows; row++) {
      x = map_rect.x;
      lineRGBA(
        screen, x, y, x + map_rect.w, y,
        255, 255, 255, 255
      );
      for (col = 0; col < map_cols; col++) {
        lineRGBA(
          screen, x, y, x, y + map_rect.h - row * TILES_SIZE,
          255, 255, 255, 255
        );
        x += TILES_SIZE;
      }
      lineRGBA(
        screen, x, y, x, y + map_rect.h - row * TILES_SIZE,
        255, 255, 255, 255
      );
      y += TILES_SIZE;
    }
    x = map_rect.x;
    lineRGBA(
      screen, x, y, x + map_rect.w, y,
      255, 255, 255, 255
    );
  }

  if (map_tile_selection.col > -1 && map_tile_selection.row > -1) {
    rectangleRGBA(
      screen,
      map_rect.x + map_tile_selection.col * TILES_SIZE,
      map_rect.y + map_tile_selection.row * TILES_SIZE,
      map_rect.x + map_tile_selection.col * TILES_SIZE + TILES_SIZE,
      map_rect.y + map_tile_selection.row * TILES_SIZE + TILES_SIZE,
      255, 0, 0, 255
    );
  }
}

void map_select_tile(int screen_x, int screen_y) {
  map_tile_selection.col = -1 * (map_rect.x - screen_x) / TILES_SIZE;
  map_tile_selection.row = -1 * (map_rect.y - screen_y) / TILES_SIZE;
  if (map_tile_selection.col >= map_cols || map_tile_selection.row >= map_rows)
    map_tile_selection_reset();
}

void map_tile_selection_reset() {
  map_tile_selection.row = -1;
  map_tile_selection.col = -1;
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
  free(map);
}
