# SPDX-License-Identifier: MIT
# Serial Terminal Module

if(CONFIG_SERIAL_TERMINAL)
  # Add include directory for the header-only library
  zephyr_include_directories(include)
endif()