#include "global.h"
#include "list.h"
#include "loader.h"

#define FALSE 0
#define TRUE 1

extern int screen_width;
extern int screen_height;
extern SDL_Surface *screen;
extern char font_name[128];

int list_length;
float list_max_y;
float list_offset[2];

struct st_list_entry list[LIST_MAX_ENTRIES];

TTF_Font *list_title_font;
TTF_Font *list_text_font;
SDL_Color list_title_color = { 255, 255, 255 };
SDL_Color list_text_color = { 202, 202, 202 };

SDL_Rect scrollbar;
SDL_Rect scrollbar_slider;
int scrollbar_active = FALSE;

void list_init() {
  list_title_font = TTF_OpenFont(font_name, 24);
  list_text_font = TTF_OpenFont(font_name, 18);

  list_length = 25;
  list_offset[0] = 0.; // x
  list_offset[1] = 0.; // y

  loader_load(list);

  int i;
  char tmp_title[128];
  for (i = 0; i < list_length; i++) {
    sprintf(tmp_title, "Entry %d:", i);
    list[i].title = TTF_RenderText_Solid(
      list_title_font, tmp_title, list_title_color
    );
    list[i].text = TTF_RenderText_Solid(
      list_text_font, "What do you expect from a list scroll?", list_text_color
    );
  }
  list_max_y = list_length * (list[i-1].title->h + list[i-1].text->h);
}

void list_change_offset(int up, float val_y) {
  if (up == TRUE) {
    if (list_offset[1] + val_y <= 0)
      list_offset[1] += val_y;
  } else {
    if (list_offset[1] - val_y >= -1 * (list_max_y - screen_height))
      list_offset[1] -= val_y;
  }
}

void list_show() {
  int i, entry_pos_x, entry_pos_y;
  entry_pos_x = (int)list_offset[0];
  entry_pos_y = (int)list_offset[1];

  SDL_Rect offset;
  for (i = 0; i < list_length; i++) {
    offset.x = entry_pos_x;
    offset.y = entry_pos_y;

    SDL_BlitSurface(list[i].title, NULL, screen, &offset);

    entry_pos_y += list[i].title->h;
    offset.y = entry_pos_y;

    SDL_BlitSurface(list[i].text, NULL, screen, &offset);

    entry_pos_y += list[i].text->h;
    offset.y = entry_pos_y;
  }
}

void list_calc_scrollbar() {
  int width = 16;
  int height = 16;
  float range_step_size = (screen_height - (5 + height)) / (list_max_y - screen_height);
  float step_pos = range_step_size * list_offset[1] * -1;

  scrollbar_slider.x = screen_width - 19;
  scrollbar_slider.y = 2 + (int)step_pos;
  scrollbar_slider.w = width;
  scrollbar_slider.h = height;

  scrollbar.x = screen_width - 21;
  scrollbar.y = 0;
  scrollbar.w = 20;
  scrollbar.h = screen_height - 1;
}

void list_scrollbar_show() {
  list_calc_scrollbar();

  rectangleRGBA(
    screen, scrollbar.x, scrollbar.y,
    scrollbar.x + scrollbar.w, scrollbar.y + scrollbar.h,
    255, 255, 255, 255
  );

  boxRGBA(
    screen,
    scrollbar_slider.x, scrollbar_slider.y,
    scrollbar_slider.x + scrollbar_slider.w, scrollbar_slider.y + scrollbar_slider.h,
    255, 255, 255, 255
  );
}

int list_set_scrollbar_active(int x, int y) {

  list_calc_scrollbar();
  if (x >= scrollbar.x && y >= scrollbar.y &&
      x <= scrollbar.x + scrollbar.w &&
      y <= scrollbar.y + scrollbar.h) {
    scrollbar_active = TRUE;
  } else {
    scrollbar_active = FALSE;
  }
  return scrollbar_active;
}

void list_move_scrollbar_slider(int y) {
  float range_step_size = (screen_height) / (list_max_y - screen_height);
  float new_list_offset = y / range_step_size * -1;
  list_offset[1] = new_list_offset;
}

void list_clean_up() {
  int i;
  for (i = 0; i < list_length; i++) {
    SDL_FreeSurface(list[i].title);
    SDL_FreeSurface(list[i].text);
  }

  TTF_CloseFont(list_title_font);
  TTF_CloseFont(list_text_font);
}
