#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define READ()                                                                 \
  ({                                                                           \
    int c;                                                                     \
    int ret = read(STDIN_FILENO, &c, 1);                                       \
    (ret > 0) ? c : -1;                                                        \
  })

#define PRINT(str) write(STDOUT_FILENO, str, strlen(str))

#include "serial_prompt.h"

int print_handler(int argc, char **argv) {
  fprintf(stdout, "print handler called\n");
  return 0;
}

int multiply_handler(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stdout, "mult num1 num2\n");
    return 0;
  }

  int num1 = atoi(argv[1]);
  int num2 = atoi(argv[2]);

  fprintf(stdout, "%d\n", num1 * num2);

  return 0;
}

cmd_t commands[] = {
    {"print", "print func", print_handler},
    {"mult", "mult num1 num2", multiply_handler},
};

int commands_count = sizeof(commands) / sizeof(cmd_t);

struct termios original_tio;

void restore_input_buffering() {
  // restore original state
  tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}

void disable_input_buffering() {
  tcgetattr(STDIN_FILENO, &original_tio);
  struct termios new_tio = original_tio;
  new_tio.c_lflag &= ~ICANON & ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void handle_interrupt(int signal) {
  restore_input_buffering();
  exit(-2);
}

int main(int argc, char **argv) {

  signal(SIGINT, handle_interrupt);
  disable_input_buffering();

  serial_greet();

  while (true)
    serial_run();

  restore_input_buffering();
}
