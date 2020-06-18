add_library(AliveDllAO SHARED dllmain.cpp)

target_include_directories(AliveDllAO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_compile_features(AliveDllAO
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

if (MSVC)
    set_target_properties(AliveDllAO PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /functionpadmin:16 /MAP")
    set_target_properties(AliveDllAO PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /functionpadmin:16 /MAP")
endif()

target_compile_definitions(AliveDllAO PRIVATE "_CRT_SECURE_NO_WARNINGS")

target_link_libraries(AliveDllAO AliveLibAO)

if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
   target_link_libraries(AliveDllAO Detours)
endif()

if (NOT DEFINED ENV{AO_ROOT})
    message(WARNING "No AO_ROOT environment variable found, resorting to the default path")
    set(AO_PATH "C:\\GOG Games\\AO" )
else()
	file(TO_CMAKE_PATH $ENV{AO_ROOT} AO_PATH)
endif()

# Ninja will die when it sees $(OutDir) as it does not understand what it means, therefore if using msvc only
# use these vars if its msvc and a native vcxproj.
if (MSVC AND CMAKE_GENERATOR MATCHES "Visual Studio")
    add_custom_command(TARGET AliveDllAO
                       POST_BUILD
                       COMMENT "Copying to game dir ${AO_PATH}"
                       COMMAND if not exist "${AO_PATH}" mkdir "${AO_PATH}"
                       COMMAND copy /y "$(OutDir)$(TargetName)$(TargetExt)" "${AO_PATH}\\AliveDllAO.dll"
                       COMMAND copy /y "$(OutDir)$(TargetName).map" "${AO_PATH}\\AliveDllAO.map")
endif()

export(TARGETS AliveDllAO FILE AliveDllAO.cmake)
install(TARGETS AliveDllAO DESTINATION .)

install(FILES "${AO_PATH}\\AliveDllAO.map" DESTINATION ".")

if (MSVC AND CMAKE_GENERATOR MATCHES "Visual Studio")
    if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        add_custom_command(TARGET AliveDllAO POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" "$(OutDir)SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" DESTINATION ".")
    else()
        add_custom_command(TARGET AliveDllAO POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" "$(OutDir)SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" DESTINATION ".")
    endif()
endif()
