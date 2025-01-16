#include <Arduino.h>
/* Step 1.
* Replace with the serial print/write function
* as per mcu
*/
#define PRINT  Serial.print
#define READ  Serial.read
/* Step 2 
* Include the header
*/
#include "serial_prompt.h"

/* Step 3
* Add your cmd handlers
* Note: command handler should has
* int <handle_name>(int argc, char **argv)
* definations.
*/
int help(int argc, char **argv)
{
    Serial.println("Serial console commands:");
    Serial.println("");
    Serial.println("help  -> this help");
    Serial.println("");
    
    return 0;
}

int multiply_handler(int argc, char **argv) {
  if (argc < 3) {
    Serial.println("mult num1 num2");
    return 0;
  }

  int num1 = atoi(argv[1]);
  int num2 = atoi(argv[2]);

  Serial.println(num1 * num2)

  return 0;
}

/* Step 4
* Add command name, description and its handler
* to commands array
*/
COMMANDS(
    {"print", "print func", print_handler},
    {"mult", "mult num1 num2", multiply_handler}
);

void setup() {
  Serial.begin(115200);

  serial_greet();
}

void loop() {
/* Step5:
* Call it periodically
*/
  serial_run();
}
