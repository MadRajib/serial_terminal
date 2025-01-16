#include <stdio.h>
#include <stdlib.h>
#include <tusb.h>
#include "pico/stdlib.h"

#define PRINT printf
#define READ()                                                                 \
  ({                                                                           \
    int c;                                                                     \
    int ret = read(STDIN_FILENO, &c, 1);                                       \
    (ret > 0) ? c : -1;                                                        \
  })

#include "serial_prompt.h"

int print_handler(int argc, char **argv) {
  fprintf(stdout, "print handler called\n");
  return 0;
}

int multiply_handler(int argc, char **argv) {
  if (argc < 3) {
    printf("mult num1 num2\n");
    return 0;
  }

  int num1 = atoi(argv[1]);
  int num2 = atoi(argv[2]);

  printf("%d\n", num1 * num2);
  return 0;
}

COMMANDS (
    {"print", "print func", print_handler},
    {"mult", "mult num1 num2", multiply_handler}
);

int main()
{
    stdio_init_all();
    while (!tud_cdc_connected()) { sleep_ms(100);  }
    serial_greet();

    while (true) {
        serial_run();
    }
}
