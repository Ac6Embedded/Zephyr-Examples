# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(EXTRA_ZEPHYR_MODULES 
        "${CMAKE_CURRENT_SOURCE_DIR}/apps/shared"
        "${CMAKE_CURRENT_SOURCE_DIR}/apps/app_a"
        "${CMAKE_CURRENT_SOURCE_DIR}/apps/app_b"
    )

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(user_mode_seperate)

target_sources(app PRIVATE src/main.c)
