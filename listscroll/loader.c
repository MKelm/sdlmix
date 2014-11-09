#include "global.h"
#include "list.h"
#include "loader.h"

#define FALSE 0
#define TRUE 1

int load_list_data(void) {
  FILE *fp = fopen("list.dat", "r");
  if (!fp)
    return FALSE;

  fseek(fp, 0L, SEEK_END);
  int f_size = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  int ch;
  char output[f_size], ch_str[2];
  while ((ch = fgetc(fp)) != EOF) {
    snprintf(ch_str, 2, "%c", ch);
    strcat(output, ch_str);
  }
  fclose(fp);

  int r;
  jsmn_parser p;
  jsmntok_t tokens[f_size/3];
  jsmn_init(&p);
  r = jsmn_parse(&p, output, strlen(output), tokens, f_size);
  if (r > 0) {
    printf("--- loaded %d tokens of %d chars\n", r, f_size);
  }

  return TRUE;
}

void loader_load(struct st_list_entry list[]) {
  load_list_data();
}
