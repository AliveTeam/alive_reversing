set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(relive_path_dump main.cpp)

target_include_directories(relive_path_dump PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../relive_api
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../3rdParty/magic_enum/include
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
target_compile_definitions(relive_path_dump PRIVATE "_CRT_SECURE_NO_WARNINGS")

if (NOT MSVC)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(relive_path_dump relive_api jsonxx_reliveapi project_warnings Threads::Threads)
else()
    target_link_libraries(relive_path_dump relive_api jsonxx_reliveapi project_warnings)
endif()

set_property(TARGET relive_path_dump PROPERTY FOLDER "ReliveAPI")
