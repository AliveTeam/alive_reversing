add_library(AliveDll SHARED dllmain.cpp)

target_include_directories(AliveDll PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_compile_features(AliveDll
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

if (MSVC)
    set_target_properties(AliveDll PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /functionpadmin:16 /MAP")
    set_target_properties(AliveDll PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /functionpadmin:16 /MAP")
endif()

target_compile_definitions(AliveDll PRIVATE "_CRT_SECURE_NO_WARNINGS")

target_link_libraries(AliveDll AliveLib)
if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
   target_link_libraries(AliveDll Detours)
endif()

if (MSVC)
    add_custom_command(TARGET AliveDll
                       POST_BUILD
                       COMMAND echo f | xcopy /s /y "$(OutDir)$(TargetName)$(TargetExt)" "C:\\GOG Games\\Abes Exoddus\\AliveDll.dll"
                       COMMAND echo f | xcopy /s /y "$(OutDir)$(TargetName).map" "C:\\GOG Games\\Abes Exoddus\\AliveDll.map"
                       COMMENT "Copying to game dir")
endif()

export(TARGETS AliveDll FILE AliveDll.cmake)
install(TARGETS AliveDll DESTINATION .)

install(FILES "C:\\GOG Games\\Abes Exoddus\\AliveDll.map" DESTINATION ".")

if (MSVC)
    if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        add_custom_command(TARGET AliveDll POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" "$(OutDir)SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" DESTINATION ".")
    else()
        add_custom_command(TARGET AliveDll POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" "$(OutDir)SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" DESTINATION ".")
    endif()
endif()
