#define STARS_AMOUNT 256

struct st_stars {
  float x; // x coordinate
  float y; // y coordinate
  float z; // z coordinate
  float zv; // z velocity
  int screen_x;
  int screen_y;
  int old_screen_x;
  int old_screen_y;
  int streak_len;
};

void stars_init();
void stars_move();
void stars_show();
