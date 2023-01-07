project(AliveLibAE VERSION 0.1 LANGUAGES CXX)

#add_library(AliveLibAE ${AliveLibAESrc})

#target_include_directories(AliveLibAE PUBLIC
#    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR},
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/json/single_include
#    $<INSTALL_INTERFACE:include>
#    PRIVATE src ${CMAKE_BINARY_DIR})

#target_compile_definitions(AliveLibAE PRIVATE "_CRT_SECURE_NO_WARNINGS")
#target_compile_definitions(AliveLibAE PRIVATE "BEHAVIOUR_CHANGE_SUB_DATA_FOLDERS")

#if (MSVC)
#    target_compile_options(AliveLibAE PRIVATE /W4 /wd4996 /WX /MP)
#
#    set_target_properties(AliveLibAE PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /functionpadmin:16")
#    set_target_properties(AliveLibAE PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /functionpadmin:16")
#endif()

#target_compile_features(AliveLibAE
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
#else()
#    if (WIN32)
#        set(WIN32_LIBS
#            Winmm
#            dsound
#            xinput
#        )
#    endif()
#endif()

#target_link_libraries(AliveLibAE
#    googletest
#    ${WIN32_LIBS}
#    ${SDL2_LIBRARIES}
#    project_warnings
#    )

#export(TARGETS AliveLibAE FILE AliveLibAE.cmake)
