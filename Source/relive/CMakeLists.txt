if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

add_executable(relive WIN32
    Exe.cpp
    resource.rc
    resource.h
    app.ico)
target_include_directories(relive PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
target_compile_features(relive
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)
target_compile_definitions(relive PRIVATE "_CRT_SECURE_NO_WARNINGS")
target_link_libraries(relive AliveLibAE AliveLibAO relive_lib project_warnings)
export(TARGETS relive FILE relive.cmake)
install(TARGETS relive DESTINATION "${BINPATH}")

if (MSVC)
    target_compile_options(relive PRIVATE /W4 /wd4996 /WX /MP)
endif()
