if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(alive_api_src
    TlvObjectBase.hpp
    AETlvs.hpp
    AOTlvs.hpp
    ITypeBase.hpp
    BasicType.hpp
    EnumType.hpp
    alive_api.cpp
    alive_api.hpp
    TypesCollection.hpp
    TypesCollection.cpp
    JsonDocument.cpp
    JsonDocument.hpp
)
add_library(alive_api ${alive_api_src})
target_link_libraries(alive_api jsonxx AliveLibAE AliveLibAO)
export(TARGETS alive_api FILE alive_api.cmake)

add_executable(alive_api_test WIN32 alive_api_test.cpp)

target_include_directories(alive_api_test PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../3rdParty/magic_enum/include
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
target_compile_features(alive_api_test
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)
target_compile_definitions(alive_api_test PRIVATE "_CRT_SECURE_NO_WARNINGS")
target_link_libraries(alive_api_test alive_api jsonxx AliveLibAE AliveLibAO)

if (MSVC AND CMAKE_GENERATOR MATCHES "Visual Studio")
    if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        add_custom_command(TARGET alive_api_test POST_BUILD
            COMMAND echo copy "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" "$(OutDir)SDL2.dll"
            COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" "$(OutDir)SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" DESTINATION ".")
    else()
        add_custom_command(TARGET alive_api_test POST_BUILD
            COMMAND echo ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" "$(OutDir)SDL2.dll"
            COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" "$(OutDir)SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" DESTINATION ".")
    endif()
endif()
export(TARGETS alive_api_test FILE alive_api_test.cmake)
install(TARGETS alive_api_test DESTINATION "${BINPATH}")
