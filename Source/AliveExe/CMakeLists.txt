if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

add_executable(AliveExe WIN32 
    Exe.cpp 
    resource.rc
    resource.h
    app.ico)
  set_target_properties(AliveExe PROPERTIES OUTPUT_NAME "relive")
target_include_directories(AliveExe PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
target_compile_features(AliveExe
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)
target_compile_definitions(AliveExe PRIVATE "_CRT_SECURE_NO_WARNINGS")
target_link_libraries(AliveExe AliveLibAE AliveLibAO)
export(TARGETS AliveExe FILE AliveExe.cmake)
install(TARGETS AliveExe DESTINATION "${BINPATH}" RENAME relive)

if (MSVC)
    target_compile_options(AliveExe PRIVATE /W4 /wd4996 /WX /MP)
endif()
