# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(minimal)
set(DTC_OVERLAY_FILE "nrf52dk_nrf52832.overlay")
target_sources(app PRIVATE src/main.c
    src/bluetooth.c)
