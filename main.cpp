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

/* Step 4
* Add command name and its handler
* to commands array
*/
cmd_t commands[] = {
  {"help", help}
};

/* keep this as it is */
int commands_count = sizeof(commands)/ sizeof(cmd_t);

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
