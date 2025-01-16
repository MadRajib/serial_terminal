#ifndef _SERIAL_PORMPT_H
#define _SERIAL_PORMPT_H
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_CLI_TEXT 80
#define MAX_WORDS 06

#define COMMANDS(...)   \
  const cmd_t commands[] = {__VA_ARGS__};\
  const size_t commands_count = sizeof(commands) / sizeof(commands[0])

static char cli_buf[MAX_CLI_TEXT] = {0};
static unsigned int cli_buf_index = 0;

static char *tokenize_buf[MAX_WORDS];
const char *wss = " \t\r\n";

typedef int (*cmd_handler_t)(int argc, char **argv);

typedef struct {
  char *name;
  char *description;
  cmd_handler_t handler;
} cmd_t;

extern const cmd_t commands[];
extern const size_t commands_count;

static inline void print_help() {
  for (int i = 0; i < commands_count; ++i) {
    PRINT(commands[i].name);
    PRINT(" : ");
    PRINT(commands[i].description);
    PRINT("\n");
  }
}

static inline int find(char *cmd) {

  if (!strcmp(cmd, "?")) {
    print_help();
    return -1;
  }

  for (int i = 0; i < commands_count; ++i) {
    if (!strcmp(cmd, commands[i].name)) {
      return i;
    }
  }

  PRINT("Unkown Command\n");
  return -1;
}

static inline int tokenize() {
  int token_count = 0;
  char *token = strtok(cli_buf, wss);

  while (token != NULL && token_count < MAX_WORDS) {
    tokenize_buf[token_count] = token;
    token_count++;
    token = strtok(NULL, wss);
  }

  return token_count;
}

static inline int read_unitl(char ch) {
  int c = 0;
  char str[] = {'\0', '\0'};
  /* timeout inside while to preserve non blocking */
  int cnt = 100;

  while ((c = READ()) > 0 && cnt > 0) {
    --cnt;
    str[0] = (char)c;

    PRINT(str);

    if (cli_buf_index > MAX_CLI_TEXT - 1) {

      /* Drop  the buffer quitely if delim is reached */
      if (str[0] == ch) {
        cli_buf_index = 0;
        return 0;
      }
      continue;
    }

    if (str[0] == ch) {
      cli_buf[cli_buf_index] = '\0';
      cli_buf_index = 0;
      return 1;
    }

    cli_buf[cli_buf_index++] = str[0];
  }

  return 0;
}

static inline void serial_greet(void) {
  PRINT("Hello from Serial Prompt\n"
        "Type '?' for help\n"
        "> ");
}

static inline void serial_run(void) {
  if (!read_unitl('\n'))
    return;

  int argc = tokenize();
  int cmd_id = -1;
  if (argc > 0 && ((cmd_id = find(tokenize_buf[0])) > -1)) {
    commands[cmd_id].handler(argc, tokenize_buf);
  }

  PRINT("> ");
}

#endif
