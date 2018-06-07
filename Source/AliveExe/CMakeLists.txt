add_executable(AliveExe WIN32 Exe.cpp)

target_include_directories(AliveExe PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_compile_features(AliveExe
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

if (MSVC)
    set_target_properties(AliveExe PROPERTIES LINK_FLAGS "/DYNAMICBASE:NO /FIXED /BASE:0x40000")
endif()

target_compile_definitions(AliveExe PRIVATE "_CRT_SECURE_NO_WARNINGS")

target_link_libraries(AliveExe AliveLib)

export(TARGETS AliveExe FILE AliveExe.cmake)
install(TARGETS AliveExe DESTINATION .)
