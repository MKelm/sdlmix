#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "stars.h"
#include <time.h>

struct st_stars stars[STARS_AMOUNT];

int stars_origin_x;
int stars_origin_y;

extern SDL_Surface *screen;

void star_init(int i) {
  stars[i].x = 0;
  stars[i].y = 0;
  stars[i].vx = -2 + (rand() % 5);
  stars[i].vy = -2 + (rand() % 5);
  int size = 2;
  stars[i].w = size;
  stars[i].h = size;
}

void stars_init(int origin_x, int origin_y) {
  stars_origin_x = origin_x;
  stars_origin_y = origin_y;
  srand(time(NULL));
  int i;
  for (i = 0; i < STARS_AMOUNT; i++) {
    star_init(i);
  }
}

void stars_move() {
  int i;
  for (i = 0; i < STARS_AMOUNT; i++) {
    stars[i].x += stars[i].vx;
    stars[i].y += stars[i].vy;

    if (abs(stars[i].x) > abs(stars_origin_x) ||
        abs(stars[i].y) > abs(stars_origin_y)) {
      star_init(i);
    }
  }
}

void stars_show() {
  int i, x, y;
  for (i = 0; i < STARS_AMOUNT; i++) {
    x = stars_origin_x - stars[i].x;
    y = stars_origin_y - stars[i].y;
    boxRGBA(
      screen, x, y, x + stars[i].w, y + stars[i].h, 255, 255, 255, 255
    );
  }
}
