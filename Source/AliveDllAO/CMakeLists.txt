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

export(TARGETS AliveDllAO FILE AliveDllAO.cmake)
install(TARGETS AliveDllAO DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")

if (WIN32)
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        add_custom_command(TARGET AliveDllAO POST_BUILD
            COMMAND echo copy "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" "$<TARGET_FILE_DIR:AliveDllAO>/SDL2.dll"
            COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" "$<TARGET_FILE_DIR:AliveDllAO>/SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")
    else()
        add_custom_command(TARGET AliveDllAO POST_BUILD
            COMMAND echo copy "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" "$<TARGET_FILE_DIR:AliveDllAO>/SDL2.dll"
            COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" "$<TARGET_FILE_DIR:AliveDllAO>/SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()
endif()
