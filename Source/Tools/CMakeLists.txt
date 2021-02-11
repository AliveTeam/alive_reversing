if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

if (WIN32)
    add_executable(vab_tool WIN32 vab_tool.cpp)

    target_include_directories(vab_tool PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        PRIVATE ${CMAKE_BINARY_DIR})
    target_compile_features(vab_tool
        PRIVATE cxx_auto_type
        PRIVATE cxx_variadic_templates)
    target_compile_definitions(vab_tool PRIVATE "_CRT_SECURE_NO_WARNINGS")
    target_link_libraries(vab_tool AliveLibAE AliveLibAO)

    export(TARGETS vab_tool FILE vab_tool.cmake)
    install(TARGETS vab_tool DESTINATION "${BINPATH}")
endif()

add_executable(alive_api WIN32 alive_api.cpp)

target_include_directories(alive_api PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
target_compile_features(alive_api
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)
target_compile_definitions(alive_api PRIVATE "_CRT_SECURE_NO_WARNINGS")
target_link_libraries(alive_api AliveLibAE AliveLibAO)

if (MSVC AND CMAKE_GENERATOR MATCHES "Visual Studio")
    if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        add_custom_command(TARGET alive_api POST_BUILD
            COMMAND echo copy "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" "$(OutDir)SDL2.dll"
            COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" "$(OutDir)SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" DESTINATION ".")
    else()
        add_custom_command(TARGET alive_api POST_BUILD
            COMMAND echo ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" "$(OutDir)SDL2.dll"
            COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" "$(OutDir)SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" DESTINATION ".")
    endif()
endif()
export(TARGETS alive_api FILE alive_api.cmake)
install(TARGETS alive_api DESTINATION "${BINPATH}")
