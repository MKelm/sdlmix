#include "SDL/SDL_gfxPrimitives.h"

void map_init();
void map_load();
void map_resize(int direction);
void map_toggle_grid();
void map_show();
void map_move(int x, int y);
void map_move_reset();
void map_select_tile(int screen_x, int screen_y);
void map_tile_selection_reset();
void map_clean_up();
