#-------------------------------------------------------------------------------
# Zephyr Example Application
#
# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(space_trash LANGUAGES C VERSION 1.0.0)

configure_file(app_version.h.in ${CMAKE_BINARY_DIR}/app/include/app_version.h)
target_include_directories(app PRIVATE ${CMAKE_BINARY_DIR}/app/include src)
target_include_directories(app PRIVATE ${CMAKE_BINARY_DIR}/app/include inc)

target_sources(app PRIVATE src/controls.c)
target_sources(app PRIVATE src/graphics.c)
target_sources(app PRIVATE src/game.c)
target_sources(app PRIVATE src/images.c)
target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/nvm.c)