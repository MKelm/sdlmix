#include "global.h"
#include "list.h"
#include "loader.h"

#define FALSE 0
#define TRUE 1

#define LOADER_CHUNK_LENGTH 1024

struct st_loader_list_entry loader_list[LIST_MAX_ENTRIES];
int loader_list_length = 0;

void loader_load_json_token(char* input, char *token, jsmntok_t *tokens, int token_idx) {
  strncpy(token, "", LOADER_CHUNK_LENGTH);
  strncpy(token, input + tokens[token_idx].start,
    tokens[token_idx].end - tokens[token_idx].start);
}

int loader_load_list_by_tokens(char *output, jsmntok_t *tokens) {
  int i = 0, j, j_max, k, k_max, li = 0;
  char chunk[LOADER_CHUNK_LENGTH];

  if (tokens[i].type == JSMN_ARRAY) {
    // iterate through list parts
    j_max = tokens[i].size;
    for (j = 0; j < j_max; j++) {
      i++;
      if (tokens[i].type == JSMN_ARRAY) {
        // iterate through list item parts
        k_max = tokens[i].size;
        for (k = 0; k < k_max; k++) {
          i++;
          if (tokens[i].type == JSMN_STRING) {
            loader_load_json_token(output, chunk, tokens, i);
            if (k == 0) {
              strncpy(loader_list[li].title, chunk, sizeof(loader_list[li].title));
            } else if (k == 1) {
              strncpy(loader_list[li].text, chunk, sizeof(loader_list[li].text));
            } else {
              strncpy(loader_list[li].image, chunk, sizeof(loader_list[li].image));
            }
          }
        }
        li++;
      }
    }
  }
  loader_list_length = li;
  return li;
}

int loader_load() {
  FILE *fp = fopen("list.dat", "r");
  if (!fp)
    return FALSE;

  fseek(fp, 0L, SEEK_END);
  int f_size = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  char output[f_size], chunk[LOADER_CHUNK_LENGTH];
  strncpy(output, "", sizeof(output));
  while (fgets(chunk, LOADER_CHUNK_LENGTH, fp) != NULL) {
    strcat(output, chunk);
  }
  fclose(fp);

  int r;
  jsmn_parser p;
  jsmntok_t tokens[f_size/3];
  jsmn_init(&p);
  r = jsmn_parse(&p, output, strlen(output), tokens, f_size);
  if (r > 0) {
    return loader_load_list_by_tokens(output, tokens);
  }
  return 0;
}
