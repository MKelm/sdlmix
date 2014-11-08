#define STARS_AMOUNT 256
#define STARS_BASE_WIDTH 10
#define STARS_BASE_HEIGHT 10

struct st_stars {
  int x;
  int y;
  int vx;
  int vy;
  int w;
  int h;
};

void stars_init();
void stars_move();
void stars_show();
