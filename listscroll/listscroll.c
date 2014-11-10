#include "global.h"
#include "list.h"

#define FALSE 0
#define TRUE 1

#define FPS 15

SDL_Surface *screen;
SDL_Surface *message;

int screen_width = 640;
int screen_height = 480;
int screen_bpp = 32;

char window_title_str[256] = "SDL 1.2 List Scroll";
char window_footer_str[256] = "developed on CubieTruck";

char font_name[128] = "beyourself.ttf";
TTF_Font *font = NULL;
SDL_Color font_color = { 255, 255, 255 };
int font_size = 24;

SDL_Event event;

int init() {
  if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    return FALSE;
  }

  screen = SDL_SetVideoMode(
    screen_width, screen_height, screen_bpp, SDL_SWSURFACE
  );
  if (screen == NULL) {
    return FALSE;
  }

  if (TTF_Init() == -1) {
    return FALSE;
  }

  SDL_WM_SetCaption(window_title_str, NULL);

  list_init();

  return TRUE;
}

int load_files() {
  font = TTF_OpenFont(font_name, font_size);
  if (font == NULL) {
    return FALSE;
  }

  return TRUE;
}

void clean_up() {
  list_clean_up();

  SDL_FreeSurface(message);

  TTF_CloseFont(font);
  TTF_Quit();

  SDL_Quit();
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
  SDL_Rect offset;
  offset.x = x;
  offset.y = y;

  SDL_BlitSurface(source, NULL, destination, &offset);
}

int main(int argc, char* args[]) {
  if (init() == FALSE) {
    return 1;
  }
  if (load_files() == FALSE) {
    return 1;
  }

  message = TTF_RenderText_Solid(font, window_footer_str, font_color);
  if (message == NULL) {
    return 1;
  }

  Uint32 frameStart = 0;
  int mouseButtonDown = FALSE;
  int quit = FALSE;
  while (quit == FALSE) {
    frameStart = SDL_GetTicks();

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_MOUSEBUTTONDOWN) {
        mouseButtonDown = TRUE;
        if (event.button.button == SDL_BUTTON_LEFT &&
            list_is_scrollbar_active() == FALSE) {
          list_select_entry_item(event.button.y);
        }
      }

      if (event.type == SDL_MOUSEBUTTONUP) {
        mouseButtonDown = FALSE;

        if (event.button.button == SDL_BUTTON_WHEELUP) {
          list_change_offset(TRUE, 10.f);
        } else if (event.button.button == SDL_BUTTON_WHEELDOWN) {
          list_change_offset(FALSE, 10.f);
        }
      }

      if (event.type == SDL_MOUSEMOTION) {
        if (list_set_scrollbar_active(event.motion.x, event.motion.y) == TRUE &&
            mouseButtonDown == TRUE) {
          list_move_scrollbar_slider(event.button.y);
        }
      }

      if (event.type == SDL_QUIT) {
        quit = TRUE;
      }
    }

    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0, 0, 0));

    list_show();
    list_scrollbar_show();

    apply_surface(
      screen_width - message->w - 25, screen_height - message->h,
      message, screen
    );

    if (SDL_Flip(screen) == -1) {
      return 1;
    }

    if ((SDL_GetTicks() - frameStart) < (1000 / FPS)) {
      SDL_Delay((1000 / FPS) - (SDL_GetTicks() - frameStart));
    }
  }

  clean_up();
  return 0;
}
