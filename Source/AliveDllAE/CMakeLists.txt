add_library(AliveDllAE SHARED dllmain.cpp)

target_include_directories(AliveDllAE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_compile_features(AliveDllAE
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

if (MSVC)
    set_target_properties(AliveDllAE PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /functionpadmin:16 /MAP")
    set_target_properties(AliveDllAE PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /functionpadmin:16 /MAP")
endif()

target_compile_definitions(AliveDllAE PRIVATE "_CRT_SECURE_NO_WARNINGS")

target_link_libraries(AliveDllAE AliveLibAE)
if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
   target_link_libraries(AliveDllAE Detours)
endif()

# Ninja will die when it sees $(OutDir) as it does not understand what it means, therefore if using msvc only
# use these vars if its msvc and a native vcxproj.
if (MSVC AND CMAKE_GENERATOR MATCHES "Visual Studio")
    add_custom_command(TARGET AliveDllAE
                       POST_BUILD
					   COMMENT "Copying to game dir $ENV{AE_ROOT}"
                       COMMAND if not exist  "$ENV{AE_ROOT}" mkdir "$ENV{AE_ROOT}"
                       COMMAND copy /y "$(OutDir)$(TargetName)$(TargetExt)" "$ENV{AE_ROOT}\\AliveDllAO.dll"
                       COMMAND copy /y "$(OutDir)$(TargetName).map" "$ENV{AE_ROOT}\\AliveDllAO.map")
endif()

export(TARGETS AliveDllAE FILE AliveDllAE.cmake)
install(TARGETS AliveDllAE DESTINATION .)

install(FILES "$ENV{AE_ROOT}\\AliveDllAE.map" DESTINATION ".")

if (MSVC AND CMAKE_GENERATOR MATCHES "Visual Studio")
    if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        add_custom_command(TARGET AliveDllAE POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" "$(OutDir)SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" DESTINATION ".")
    else()
        add_custom_command(TARGET AliveDllAE POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" "$(OutDir)SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" DESTINATION ".")
    endif()
endif()
