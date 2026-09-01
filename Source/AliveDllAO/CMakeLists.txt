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
    add_custom_command(TARGET AliveDllAO POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:SDL3::SDL3>"
            "$<TARGET_FILE_DIR:AliveDllAO>")

    install(FILES
        "$<TARGET_FILE:SDL3::SDL3>"
        DESTINATION .
    )
endif()
