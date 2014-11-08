#define STARS_AMOUNT 256
#define STARS_SIZE 1

struct st_stars {
  float x; // x coordinate
  float y; // y coordinate
  float z; // z coordinate
  float zv; // z velocity
  int screen_x;
  int screen_y;
};

void stars_init();
void stars_move();
void stars_show();
