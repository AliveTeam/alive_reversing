cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

option(RENDER_TEST "Create test object that renders all prim types on boot" OFF)
option(DEVELOPER_MODE "Boot direct to main selection screen, enable debug.sav loading" ON)
option(BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE "Force game to run in windowed mode" ON)
option(BEHAVIOUR_CHANGE_SUB_DATA_FOLDERS "Allow the game to load ddv and lvl files from their own folders. (movies, levels)" ON)
option(FORCE_DDCHEAT "Force ddcheat mode to be enabled" ON)
option(LCD_PS1_SPEED "Corrects LCD Screens to move as fast as the original PS1 version of the game." OFF)
option(XINPUT_SUPPORT "Adds XINPUT support to the game and replaces in game fonts with Xbox Versions." OFF)
option(USE_SDL2 "Use SDL2 instead of Win32 APIs." ON)
option(USE_SDL2_SOUND "Use SDL2 for audio." ON)
option(ORIGINAL_PS1_BEHAVIOR "Fixes bugs in the PSX Emu layer / Gameplay to match PS1 version of the game." ON)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Source/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Source/AliveLib/config.h)
