cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

option(RENDER_TEST "Create test object that renders all prim types on boot" OFF)
option(DEVELOPER_MODE "Boot direct to main selection screen, enable debug.sav loading" ON)
option(BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE "Force game to run in windowed mode" ON)
option(FORCE_DDCHEAT "Force ddcheat mode to be enabled" ON)
option(LCD_PS1_SPEED "Corrects LCD Screens to move as fast as the original PS1 version of the game." OFF)
option(XINPUT_SUPPORT "Adds XINPUT support to the game and replaces in game fonts with Xbox Versions." OFF)
option(USE_SDL2 "Use SDL2 instead of Win32 APIs." ON)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/../Source/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)
