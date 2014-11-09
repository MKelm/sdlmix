#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "list.h"

#define FALSE 0
#define TRUE 1

struct st_list_entry list[LIST_MAX_ENTRIES];
int list_length;
float list_offset[2];

extern SDL_Surface *screen;
extern char font_name[128];

struct st_list_entry_format list_title_format;
struct st_list_entry_format list_text_format;

void list_init() {

  list_title_format.color[0] = 255;
  list_title_format.color[1] = 255;
  list_title_format.color[2] = 255;
  list_title_format.size = 24;

  list_text_format.color[0] = 202;
  list_text_format.color[1] = 202;
  list_text_format.color[2] = 202;
  list_text_format.size = 18;

  list_offset[0] = 0; // x
  list_offset[1] = 0; // y
  list_length = 25;
  int i;
  for (i = 0; i < list_length; i++) {
    strncpy(list[i].title, "Entry i:", sizeof(list[i].title));
    strncpy(list[i].text, "This is an entry in the list", sizeof(list[i].text));
  }
}

void list_change_offset(int up, float y_val) {
  if (up == TRUE)
    list_offset[1] -= y_val;
  else
    list_offset[1] += y_val;
}

void list_show() {
  int i, entry_pos_x, entry_pos_y;
  entry_pos_x = (int)list_offset[0];
  entry_pos_y = (int)list_offset[1];

  TTF_Font *list_title_font;
  list_title_font = TTF_OpenFont(font_name, list_title_format.size);

  SDL_Surface *title;
  SDL_Color title_color = {
    list_title_format.color[0], list_title_format.color[1], list_title_format.color[2]
  };

  TTF_Font *list_text_font;
  list_text_font = TTF_OpenFont(font_name, list_text_format.size);

  SDL_Surface *text;
  SDL_Color text_color = {
    list_text_format.color[0], list_text_format.color[1], list_text_format.color[2]
  };

  for (i = 0; i < list_length; i++) {
    SDL_Rect offset;
    offset.x = entry_pos_x;
    offset.y = entry_pos_y;

    title = TTF_RenderText_Solid(list_title_font, list[i].title, title_color);
    SDL_BlitSurface(title, NULL, screen, &offset);

    entry_pos_y += title->h;
    offset.y = entry_pos_y;

    text = TTF_RenderText_Solid(list_text_font, list[i].text, text_color);
    SDL_BlitSurface(text, NULL, screen, &offset);

    entry_pos_y += text->h;
    offset.y = entry_pos_y;
  }
}