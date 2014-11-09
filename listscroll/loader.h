struct st_loader_list_entry {
  char title[128];
  char text[512];
  char image[128];
};

void loader_load_json_token(char* input, char *token, jsmntok_t *tokens, int token_idx);
int loader_load_list_by_tokens(char *output, jsmntok_t *tokens);
int loader_load();
