#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "stars.h"
#include <time.h>

struct st_stars stars[STARS_AMOUNT];

int stars_origin_x;
int stars_origin_y;

extern SDL_Surface *screen;
extern int const screen_width;
extern int const screen_height;

void star_init(int i) {

  stars[i].x = -500 + rand() % 1001;
  stars[i].y = -500 + rand() % 1001;
  stars[i].z = 100 + rand() % 901;

  int rzv = 500 + (rand() % 5001);
  stars[i].zv = rzv / 1000.0;

  stars[i].size = 2;
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

    stars[i].z -= stars[i].zv;

    stars[i].screen_x = stars[i].x / stars[i].z * 100 + stars_origin_x;
    stars[i].screen_y = stars[i].y / stars[i].z * 100 + stars_origin_y;

    if (stars[i].screen_x < 0 || stars[i].screen_y < 0 ||
        stars[i].screen_x > screen_width || stars[i].screen_y > screen_height ||
        stars[i].z < 1) {
      star_init(i);
    }
  }
}

void stars_show() {
  int i, x, y;
  for (i = 0; i < STARS_AMOUNT; i++) {
    x = stars[i].screen_x;
    y = stars[i].screen_y;

    int alpha = (255/5) * stars[i].zv;
    boxRGBA(
      screen, x, y, x + stars[i].size, y + stars[i].size, 255, 255, 255, alpha
    );
  }
}
