#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <time.h>

#define FALSE 0
#define TRUE 1

#define FPS 15

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32

#define STARS_AMOUNT 256
#define STARS_BASE_WIDTH 10
#define STARS_BASE_HEIGHT 10

SDL_Surface *screen;
SDL_Surface *message;

int origin_x;
int origin_y;

struct st_stars {
  float x;
  float y;
  float z;
  float z_max;
  float w;
  float h;
  float dx;
  float dy;
  float dz;
  float ddx;
  float ddy;
} stars[STARS_AMOUNT];

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

void star_init(int i) {
  int size = (rand() % STARS_BASE_WIDTH) + 3;
  stars[i].w = size;
  stars[i].h = size;
  stars[i].x = rand() % SCREEN_WIDTH - origin_x;
  stars[i].y = rand() % SCREEN_HEIGHT - origin_y;
  stars[i].z = (SCREEN_WIDTH > SCREEN_HEIGHT) ? SCREEN_WIDTH : SCREEN_HEIGHT;
  stars[i].z_max = stars[i].z;

  int x_coeff = stars[i].x > 0 ? 1 : -1;
  int y_coeff = stars[i].y > 0 ? 1 : -1;

  if (abs(stars[i].x) > abs(stars[i].y)) {
    stars[i].dx = 1.0;
    stars[i].dy = abs(stars[i].y / stars[i].x);
  } else {
    stars[i].dx = abs(stars[i].x / stars[i].y);
    stars[i].dy = 1.0;
  }
  stars[i].dx *= (float)x_coeff;
  stars[i].dy *= (float)y_coeff;
  stars[i].dz = -1.0;

  stars[i].ddx = .1 * stars[i].dx;
  stars[i].ddy = .1 * stars[i].dy;
}

void stars_init() {
  srand(time(NULL));
  int i;
  for (i = 0; i < STARS_AMOUNT; i++) {
    star_init(i);
  }
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

  origin_x = SCREEN_WIDTH / 2;
  origin_y = SCREEN_HEIGHT / 2;

  stars_init();

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

void stars_show() {
  int i, x, y;
  for (i = 0; i < STARS_AMOUNT; i++) {
    x = origin_x + stars[i].x - stars[i].w / 2;
    y = origin_y + stars[i].y - stars[i].h / 2;
    boxRGBA(
      screen, x, y, x + stars[i].w, y + stars[i].h, 255, 255, 255, 255
    );
  }
}

void stars_move() {
  int i;
  for (i = 0; i < STARS_AMOUNT; i++) {
    stars[i].x += stars[i].dx;
    stars[i].y += stars[i].dy;
    stars[i].z += stars[i].dz;

    stars[i].dx += stars[i].ddx;
    stars[i].dy += stars[i].ddy;

    float size = STARS_BASE_WIDTH + (stars[i].z_max - stars[i].z) * 0.1;
    stars[i].w = size;
    stars[i].h = size;

    if (abs(stars[i].x) > abs(origin_x) ||
        abs(stars[i].y) > abs(origin_y)) {
      star_init(i);
    }
  }
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
