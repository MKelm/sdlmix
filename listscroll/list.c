#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "list.h"

#define FALSE 0
#define TRUE 1

#define LIST_MAX_ENTRIES 1024

extern int screen_width;
extern int screen_height;
extern SDL_Surface *screen;
extern char font_name[128];

int list_length;
float list_max_y;
float list_offset[2];

struct st_list_entry {
  SDL_Surface *title;
  SDL_Surface *text;
} list[LIST_MAX_ENTRIES];

TTF_Font *list_title_font;
TTF_Font *list_text_font;
SDL_Color list_title_color = { 255, 255, 255 };
SDL_Color list_text_color = { 202, 202, 202 };

SDL_Rect scroll_slider;
int scroll_slider_active = FALSE;

void list_init() {
  list_title_font = TTF_OpenFont(font_name, 24);
  list_text_font = TTF_OpenFont(font_name, 18);

  list_length = 25;
  list_offset[0] = 0.; // x
  list_offset[1] = 0.; // y

  int i;
  for (i = 0; i < list_length; i++) {
    list[i].title = TTF_RenderText_Solid(
      list_title_font, "Entry i:", list_title_color
    );
    list[i].text = TTF_RenderText_Solid(
      list_text_font, "This is an entry in the list", list_text_color
    );
  }
  list_max_y = list_length * (list[i-1].title->h + list[i-1].text->h);
}

void list_change_offset(int up, float val_y) {
  if (up == TRUE)
    list_offset[1] += val_y;
  else
    list_offset[1] -= val_y;
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

void list_calc_scroll_slider() {
  int width = 15;
  int height = 15;
  float range_step_size = (screen_height - (5 + height)) / (list_max_y - screen_height);
  float step_pos = range_step_size * list_offset[1] * -1;

  scroll_slider.x = screen_width - 18;
  scroll_slider.y = 2 + (int)step_pos;
  scroll_slider.w = width;
  scroll_slider.h = height;
}

void list_scrollbar_show() {

  rectangleRGBA(
    screen, screen_width - 20, 0, screen_width - 1, screen_height - 1,
    255, 255, 255, 255
  );

  list_calc_scroll_slider();
  boxRGBA(
    screen,
    scroll_slider.x, scroll_slider.y,
    scroll_slider.x + scroll_slider.w, scroll_slider.y + scroll_slider.h,
    255, 255, 255, 255
  );
}

int list_scroll_slider_active(int x, int y) {

  list_calc_scroll_slider();
  if (x >= scroll_slider.x && y >= scroll_slider.y &&
      x <= scroll_slider.x + scroll_slider.w &&
      y <= scroll_slider.y + scroll_slider.h) {
    scroll_slider_active = TRUE;
  } else {
    scroll_slider_active = FALSE;
  }
  return scroll_slider_active;
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
