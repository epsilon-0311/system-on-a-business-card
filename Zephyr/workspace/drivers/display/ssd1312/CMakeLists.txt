# SPDX-License-Identifier: Apache-2.0

if(CONFIG_SSD1312)
  # Add driver .h-fukes to the set of global include paths.
  zephyr_include_directories(.)

  zephyr_library()
  zephyr_library_sources(
    ssd1312.c
    )
endif()