#define LIST_MAX_ENTRIES 1024

struct st_list_entry {
  SDL_Surface *title;
  SDL_Surface *text;
  SDL_Surface *image;
};

void list_init();
void list_change_offset(int up, float val_y);
void list_show();
void list_calc_scroll_slider();
void list_scrollbar_show();
void list_clean_up();

int list_set_scrollbar_active(int x, int y);
void list_move_scrollbar_slider(int y);
