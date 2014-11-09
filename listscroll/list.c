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
extern struct st_loader_list_entry loader_list[LIST_MAX_ENTRIES];

TTF_Font *list_title_font;
TTF_Font *list_text_font;
SDL_Color list_title_color = { 255, 255, 255 };
SDL_Color list_text_color = { 202, 202, 202 };

SDL_Rect scrollbar;
SDL_Rect scrollbar_slider;
int scrollbar_active = FALSE;

int image_size = 32;
int image_margin_right = 4;
int title_margin_bottom = 1;
int text_margin_bottom = 4;

SDL_Surface *list_load_image(char *filename) {
  SDL_Surface* loadedImage = NULL;
  SDL_Surface* optimizedImage = NULL;

  loadedImage = IMG_Load(filename);
  if (loadedImage != NULL) {

    optimizedImage = SDL_DisplayFormat(loadedImage);
    SDL_FreeSurface(loadedImage);
    if (optimizedImage != NULL) {
      SDL_SetColorKey(
        optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 0, 0)
      );
    }
  }

  return optimizedImage;
}

void list_init() {
  list_title_font = TTF_OpenFont(font_name, 18);
  list_text_font = TTF_OpenFont(font_name, 16);

  list_offset[0] = 0.; // x
  list_offset[1] = 0.; // y

  list_length = loader_load();

  int i;
  for (i = 0; i < list_length; i++) {
    list[i].title = TTF_RenderText_Solid(
      list_title_font, loader_list[i].title, list_title_color
    );
    list[i].text = TTF_RenderText_Solid(
      list_text_font, loader_list[i].text, list_text_color
    );
    if (strcmp(loader_list[i].image, "null") == 0) {
      list[i].image = NULL;
    } else {
      list[i].image = list_load_image(loader_list[i].image);
    }
  }
  list_max_y = list_length *
    (list[i-1].title->h + title_margin_bottom + list[i-1].text->h + text_margin_bottom);
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

    if (list[i].image != NULL) {
      SDL_BlitSurface(list[i].image, NULL, screen, &offset);
      offset.x += image_size + image_margin_right;
    } else {
      offset.x = entry_pos_x;
    }

    SDL_BlitSurface(list[i].title, NULL, screen, &offset);

    entry_pos_y += list[i].title->h + title_margin_bottom;
    offset.y = entry_pos_y;

    SDL_BlitSurface(list[i].text, NULL, screen, &offset);

    entry_pos_y += list[i].text->h + text_margin_bottom;
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
