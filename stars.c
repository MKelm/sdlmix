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

int stars_rand_min_max(int min, int max) {
  return (rand() % (max+1-min))+min;
}

void star_init(int i) {
  stars[i].x = stars_rand_min_max(500, 2000) - 1000;
  stars[i].y = stars_rand_min_max(500, 2000) - 1000;
  stars[i].z = stars_rand_min_max(100, 1000);

  stars[i].screen_x = stars[i].old_screen_x = 0;
  stars[i].screen_y = stars[i].old_screen_y = 0;

  stars[i].zv = stars_rand_min_max(500, 5000) / 1000.0;
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

    if (stars[i].screen_x != 0 && stars[i].screen_y != 0) {
      stars[i].old_screen_x = stars[i].screen_x;
      stars[i].old_screen_y = stars[i].screen_y;
    }

    stars[i].screen_x = stars[i].x / stars[i].z * 100 + stars_origin_x;
    stars[i].screen_y = stars[i].y / stars[i].z * 100 + stars_origin_y;

    if (stars[i].old_screen_x != 0 && stars[i].old_screen_y != 0) {
      float xd = stars[i].screen_x - stars[i].old_screen_x;
      float yd = stars[i].screen_y - stars[i].old_screen_y;
      stars[i].streak_len = sqrt(xd * xd + yd * yd);
    } else {
      stars[i].old_screen_x = stars[i].screen_x;
      stars[i].old_screen_y = stars[i].screen_y;
    }

    if (stars[i].screen_x < 0 || stars[i].screen_y < 0 ||
        stars[i].screen_x > screen_width || stars[i].screen_y > screen_height ||
        stars[i].z < 1) {
      star_init(i);
    }
  }
}

void stars_show() {
  int i;
  for (i = 0; i < STARS_AMOUNT; i++) {

    int alpha = (int)((5000 * stars[i].zv) / stars[i].z);
    if (alpha > 255) alpha = 255;
    lineRGBA(
      screen, stars[i].screen_x, stars[i].screen_y,
      stars[i].screen_x, stars[i].screen_y,
      255, 255, 255, alpha
    );

    if (stars[i].streak_len > 1) alpha = (alpha / stars[i].streak_len);
    lineRGBA(
      screen, stars[i].screen_x, stars[i].screen_y,
      stars[i].old_screen_x, stars[i].old_screen_y,
      255, 255, 255, alpha
    );
  }
}
