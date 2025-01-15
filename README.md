# serial_terminal
A terminal for micro-controllers.

Include **serial_prompt.h** header file in your project to use it.

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
  Add your cmd handlers,
  Note: command handler should has
  **int <handle_name>(int argc, char **argv)** definations.
  ```c
  int sample(int argc, char **argv)
  {
      Serial.println("This is sample cmd");
      return 0;
  }
  ```
* Step 4
  Add command nam, description and its handler to commands array
  ```c
  cmd_t commands[] = {
    {"sample", "sample cmd description", sample}
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
* Step 6 Flash the firmware to the device, you should see following ouput
  ```bash
  Hello from Serial Prompt
  Type '?' for help
  >
  ```
  If you type "*" you should see the list of cmd available
  ```bash
  > ?
  sample : sample cmd description
  ```
