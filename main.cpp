#include <Arduino.h>
#define PRINT  Serial.print
#define READ  Serial.read
#include "serial_prompt.h"

int help(int argc, char **argv)
{
    Serial.println("Serial console commands:");
    Serial.println("");
    Serial.println("help  -> this help");
    Serial.println("");
    
    return 0;
}

cmd_t commands[] = {
  {"help", help}
};

int commands_count = sizeof(commands)/ sizeof(cmd_t);

void setup() {
  Serial.begin(115200);
  serial_greet();
}

void loop() {
  serial_run();
}