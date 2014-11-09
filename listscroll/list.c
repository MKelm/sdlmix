#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "list.h"

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

SDL_Surface *list_title;
SDL_Surface *list_text;

void list_init() {
  list_title_font = TTF_OpenFont(font_name, 24);
  list_text_font = TTF_OpenFont(font_name, 18);

  list_length = 25;
  list_max_y = 0.;
  list_offset[0] = 0.; // x
  list_offset[1] = 0.; // y

  int i;
  for (i = 0; i < list_length; i++) {
    strncpy(list[i].title, "Entry i:", sizeof(list[i].title));
    strncpy(list[i].text, "This is an entry in the list", sizeof(list[i].text));
  }
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

    list_title = TTF_RenderText_Solid(list_title_font, list[i].title, list_title_color);
    SDL_BlitSurface(list_title, NULL, screen, &offset);

    entry_pos_y += list_title->h;
    offset.y = entry_pos_y;

    list_text = TTF_RenderText_Solid(list_text_font, list[i].text, list_text_color);
    SDL_BlitSurface(list_text, NULL, screen, &offset);

    entry_pos_y += list_text->h;
    offset.y = entry_pos_y;

    SDL_FreeSurface(list_title);
    SDL_FreeSurface(list_text);
  }
  list_max_y = list_length * (list_title->h + list_text->h);

}

void list_scrollbar_show() {

  float range_step_size = (screen_height - 12 -13) / (list_max_y - screen_height);

  float step_pos = range_step_size * list_offset[1] * -1;

  rectangleRGBA(
    screen, screen_width - 20, 0, screen_width - 1, screen_height - 1,
    255, 255, 255, 255
  );

  boxRGBA(
    screen,
    screen_width - 18, 2 + (int)step_pos,
    screen_width - 3, 20 + (int)step_pos,
    255, 255, 255, 255
  );
}

void list_clean_up() {
  TTF_CloseFont(list_title_font);
  TTF_CloseFont(list_text_font);
}
