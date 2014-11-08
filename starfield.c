#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "stars.h"

#define FALSE 0
#define TRUE 1

#define FPS 15

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32

SDL_Surface *screen;
SDL_Surface *message;

TTF_Font *font = NULL;
SDL_Color font_color = { 226, 0, 0 };
int font_size = 42;

SDL_Event event;

SDL_Surface *load_image(char *filename) {
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

int init() {
  if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    return FALSE;
  }

  screen = SDL_SetVideoMode(
    SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE
  );
  if (screen == NULL) {
    return FALSE;
  }

  if (TTF_Init() == -1) {
    return FALSE;
  }

  SDL_WM_SetCaption("SDL Starfield", NULL);

  stars_init(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

  return TRUE;
}

int load_files() {
  font = TTF_OpenFont("beyourself.ttf", font_size);
  if (font == NULL) {
    return FALSE;
  }

  return TRUE;
}

void clean_up() {
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

  message = TTF_RenderText_Solid(font, "SDL 1.2 Starfield", font_color);
  if (message == NULL) {
    return 1;
  }

  Uint32 frameStart = 0;
  int quit = FALSE;
  while (quit == FALSE) {
    frameStart = SDL_GetTicks();

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = TRUE;
      }
    }

    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0, 0, 0));
    stars_show();
    apply_surface(
      SCREEN_WIDTH / 2 - message->w / 2, SCREEN_HEIGHT / 2 - message->h / 2,
      message, screen
    );

    if (SDL_Flip(screen) == -1) {
      return 1;
    }

    if ((SDL_GetTicks() - frameStart) < (1000 / FPS)) {
      SDL_Delay((1000 / FPS) - (SDL_GetTicks() - frameStart));
    }

    stars_move();
  }

  clean_up();
  return 0;
}
