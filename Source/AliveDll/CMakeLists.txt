add_library(AliveDll SHARED dllmain.cpp)

target_include_directories(AliveDll PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_compile_features(AliveDll
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

if (MSVC)
    set_target_properties(AliveDll PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(AliveDll PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
endif()

target_link_libraries(AliveDll AliveLib)

add_custom_command(TARGET AliveDll
                   POST_BUILD
                   COMMAND echo f | xcopy /s/y "$(OutDir)$(TargetName)$(TargetExt)" "C:\\Program Files (x86)\\Abe Exoddus\\ar.dll"
                   COMMENT "Copying to game dir")

export(TARGETS AliveDll FILE AliveDll.cmake)
install(TARGETS AliveDll DESTINATION .)
