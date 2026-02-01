#ifndef _SERIAL_PORMPT_H
#define _SERIAL_PORMPT_H
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_CLI_TEXT 80
#define MAX_WORDS 06
#define BACKSPACE_CHAR 0x08
#define END_OF_STR_CHAR '\0'
#define HELP_STR "?"
#define ESCAPE_CHAR '\x1b'
#define CSI_CHAR '['

#define COMMANDS(...)   \
  const cmd_t commands[] = {__VA_ARGS__};\
  const size_t commands_count = sizeof(commands) / sizeof(commands[0])

static char cli_buf[MAX_CLI_TEXT] = {0};
static unsigned int cli_buf_index = 0;
static unsigned int cursor = 0;

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

  if (!strcmp(cmd, HELP_STR)) {
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

static inline void redraw(void) {
  PRINT("\x1b[s");
  PRINT("\x1b[K");
  char str[2] = {END_OF_STR_CHAR, END_OF_STR_CHAR};
  for (int i = cursor; i < cli_buf_index; i++) {
    str[0] = cli_buf[i];
    PRINT(str);
  }
  PRINT(" \x1b[u");
}

static inline int read_unitl(char ch) {
  int c = 0;
  char str[] = {END_OF_STR_CHAR, END_OF_STR_CHAR};
  /* timeout inside while to preserve non blocking */
  int cnt = 100;

  while ((c = READ()) > 0 && cnt > 0) {
    --cnt;
    str[0] = (char)c;

    /* Handle backspace */
    if (str[0] == BACKSPACE_CHAR) { /* handle backspace key */
      if (cursor) {
        memmove(&cli_buf[cursor - 1], &cli_buf[cursor], cli_buf_index - cursor);
        cli_buf_index--;
        cursor--;
        PRINT("\x1b[D");
        redraw();
      }
    } else if (str[0] == ESCAPE_CHAR) {
      char seq[2];
      seq[0] = READ();
      seq[1] = READ();

      if (seq[0] == CSI_CHAR) {
        if (seq[1] == 'D' && cursor > 0) { /* handle left key */
          PRINT("\x1b[D");
          cursor--;
        } else if(seq[1] == 'C' && cursor < cli_buf_index) { /* handle right key */
          PRINT("\x1b[C");
          cursor++;
        } else if(seq[1] == '3') { /* handle delete key */
          seq[1] = READ();
          if (seq[1] == '~' && cursor < cli_buf_index) {
            memmove(&cli_buf[cursor], &cli_buf[cursor + 1], cli_buf_index - cursor - 1);
            cli_buf_index--;
            redraw();
          }
        }
      }
    } else if (str[0] >= 32 && str[0] <= 126) {
      /* handle printable chars */

      /* When character is added at the end of the word */
      if (cli_buf_index < MAX_CLI_TEXT - 1 && cursor == cli_buf_index) {

        PRINT(str);
        cli_buf[cli_buf_index++] = str[0];
        cursor++;
      /* When character is added in between */
      } else if (cli_buf_index < MAX_CLI_TEXT - 1) {

        memmove(&cli_buf[cursor + 1], &cli_buf[cursor], cli_buf_index - cursor);
        cli_buf[cursor] = str[0];
        cli_buf_index++;
        PRINT(str);
        cursor++;
        redraw();

      }
    } else if (str[0] == ch) {
      PRINT("\n");
      cli_buf[cli_buf_index] = END_OF_STR_CHAR;

      cli_buf_index = 0;
      cursor = 0;
      return 1;
    }
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
