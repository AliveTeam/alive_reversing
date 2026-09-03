if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

add_executable(relive MACOSX_BUNDLE WIN32
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
target_link_libraries(relive relive_lib relive_ipc project_warnings)
export(TARGETS relive FILE relive.cmake)
install(TARGETS relive DESTINATION "${BINPATH}" BUNDLE DESTINATION . RUNTIME DESTINATION .)

if (MSVC)
    target_compile_options(relive PRIVATE /W4 /wd4996 /WX /MP)
endif()

if (MSVC AND CMAKE_GENERATOR MATCHES "Visual Studio")
    if (WIN32)
        add_custom_command(TARGET AliveDllAE POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "$<TARGET_FILE:SDL3::SDL3>"
                "$<TARGET_FILE_DIR:relive>")

        install(FILES
                "$<TARGET_FILE:SDL3::SDL3>"
                DESTINATION .
        )
    endif()
endif()


if (APPLE)
    # Fix link path to look in the bundle
add_custom_command(TARGET relive POST_BUILD
                   COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2.dylib $<TARGET_FILE:relive>
                   COMMENT "Fixing up libSDL2.dylib rpath")
   
    INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../build/Frameworks/libSDL2.dylib" DESTINATION "relive.app/Contents/Frameworks")
    INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../build/Frameworks/libMoltenVK.dylib" DESTINATION "relive.app/Contents/Frameworks")
endif()
