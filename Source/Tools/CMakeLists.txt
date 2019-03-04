add_executable(Tools WIN32 
    Tools.cpp)

target_include_directories(Tools PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})

target_compile_features(Tools
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

target_compile_definitions(Tools PRIVATE "_CRT_SECURE_NO_WARNINGS")

target_link_libraries(Tools AliveLib)

export(TARGETS Tools FILE Tools.cmake)
install(TARGETS Tools DESTINATION .)
