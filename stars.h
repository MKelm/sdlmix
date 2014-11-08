#define STARS_AMOUNT 256
#define STARS_BASE_WIDTH 10
#define STARS_BASE_HEIGHT 10

struct st_stars {
  float x;
  float y;
  float z;
  int screen_x;
  int screen_y;
  int size;
};

void stars_init();
void stars_move();
void stars_show();
