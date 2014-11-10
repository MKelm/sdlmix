#define LIST_MAX_ENTRIES 1024

struct st_list_entry {
  SDL_Surface *title;
  SDL_Surface *text;
  SDL_Surface *image;
};

struct st_list_options {
  int length;
  int selected_item;
  float offset_x;
  float offset_y;
  float length_y;
};

void list_init();
void list_change_offset(int up, float val_y);
void list_show();
void list_calc_scrollbar();
void list_scrollbar_show();
void list_clean_up();

int list_is_scrollbar_active();
int list_set_scrollbar_active(int x, int y);

void list_move_scrollbar_slider(int screen_y);
void list_select_entry_item(int screen_y);
