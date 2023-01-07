project(AliveLibAO VERSION 0.1 LANGUAGES CXX)


#add_library(AliveLibAO ${AliveLibSrcAO})

#target_include_directories(AliveLibAO PUBLIC
#    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR},
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/json/single_include>
#    $<INSTALL_INTERFACE:include>
#    PRIVATE src ${CMAKE_BINARY_DIR})

#target_compile_definitions(AliveLibAO PRIVATE "_CRT_SECURE_NO_WARNINGS")

#if (MSVC)
#    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
#        target_compile_options(AliveLibAO PRIVATE /W4 /wd4996 /WX /MP)
#    else()
#        target_compile_options(AliveLibAO PRIVATE /W4 /wd4996 /MP)
#    endif()
#
#    set_target_properties(AliveLibAO PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /functionpadmin:16")
#    set_target_properties(AliveLibAO PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /functionpadmin:16")
#endif()

#target_compile_features(AliveLibAO
#    PRIVATE cxx_strong_enums
#    PRIVATE cxx_static_assert
#    PRIVATE cxx_lambdas
#    PRIVATE cxx_nullptr
#    PRIVATE cxx_range_for
#    PRIVATE cxx_decltype
#    PRIVATE cxx_auto_type
#    PRIVATE cxx_variadic_templates)

#if (MINGW)
#    set(WIN32_LIBS "imm32")
#    set(AE_LIB AliveLibAE)
#else()
#    if (WIN32)
#        set(WIN32_LIBS
#            Winmm
#            dsound
#            xinput
#        )
#    endif()
#endif()

#target_link_libraries(AliveLibAO
#    googletest
#    Cinder
#    libglew_static
#    imgui
#    glm
#    ${WIN32_LIBS}
#    ${SDL2_LIBRARIES}
#    project_warnings
#    )

#export(TARGETS AliveLibAO FILE AliveLibAO.cmake)
