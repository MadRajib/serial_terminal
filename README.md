# serial_terminal
A terminal for micro-controllers 

* Step 1
  Replace with the serial print/write function, here I have taken
  example for Arduino framework.
  ```c
  #define PRINT  Serial.print
  #define READ  Serial.read
  ```
* Step 2
  Include the header
  ```c
  #define PRINT  Serial.print
  #define READ  Serial.read
  #include "serial_prompt.h"
  ```
* Step 3
  Add your cmd handlers
  Note: command handler should has
  **int <handle_name>(int argc, char **argv)** definations.
  ```c
  int help(int argc, char **argv)
  {
      Serial.println("Serial console commands:");
      Serial.println("");
      Serial.println("help  -> this help");
      Serial.println("");
      
      return 0;
  }
  ```
* Step 4
  Add command name and its handler to commands array
  ```c
  cmd_t commands[] = {
    {"help", help}
  };
  /* keep this as it is */
  int commands_count = sizeof(commands)/ sizeof(cmd_t);
  ```
* Step 5
  Call it periodically
  ```c
  void loop() {
    serial_run();
  }
  ```
