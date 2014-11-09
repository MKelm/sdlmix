#define LIST_MAX_ENTRIES 1024

struct st_list_entry {
  char title[128];
  char text[512];
};

struct st_list_entry_format {
  int color[3];
  int size;
};

void list_init();
void show_list();
