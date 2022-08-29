if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(relive_api_unit_test relive_api_unit_test.cpp)
add_test(api_unit_test relive_api_unit_test)

if (MSVC)
    target_compile_options(relive_api PRIVATE /W4 /wd4996 /WX /MP)
    target_compile_options(relive_api_unit_test PRIVATE /W4 /wd4996 /WX /MP)
endif()

target_include_directories(relive_api_unit_test PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../3rdParty/magic_enum/include
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
target_compile_features(relive_api_unit_test
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)
target_compile_definitions(relive_api_unit_test PRIVATE "_CRT_SECURE_NO_WARNINGS")

if (NOT MSVC)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(relive_api_unit_test relive_api relive_lib googletest jsonxx_reliveapi libglew_static project_warnings Threads::Threads)
else()
    target_link_libraries(relive_api_unit_test relive_api relive_lib googletest jsonxx_reliveapi libglew_static project_warnings)
endif()

export(TARGETS relive_api_unit_test FILE relive_api_unit_test.cmake)
set_property(TARGET relive_api_unit_test PROPERTY FOLDER "ReliveAPI")
install(TARGETS relive_api_unit_test DESTINATION "${BINPATH}")
