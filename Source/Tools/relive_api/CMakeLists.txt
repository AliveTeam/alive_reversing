if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(relive_api_src
    TlvObjectBase.hpp
    PropertyCollection.cpp
    AETlvs.hpp
    AOTlvs.hpp
    ITypeBase.hpp
    BasicType.hpp
    EnumType.hpp
    relive_api.cpp
    relive_api.hpp
    TypesCollection.hpp
    TypesCollection.cpp
    TypesCollectionAO.cpp
    TypesCollectionAE.cpp
    JsonDocument.cpp
    JsonDocument.hpp
    JsonUpgrader.hpp
    JsonUpgrader.cpp
    AOJsonUpgrader.hpp
    AOJsonUpgrader.cpp
    AEJsonUpgrader.hpp
    AEJsonUpgrader.cpp
    TypedProperty.hpp
    LvlReaderWriter.hpp
    ByteStream.hpp
)
add_library(relive_api ${relive_api_src})
target_link_libraries(relive_api jsonxx AliveLibAE AliveLibAO)
export(TARGETS relive_api FILE relive_api.cmake)


add_executable(relive_api_test relive_api_test.cpp)

if (MSVC)
    target_compile_options(relive_api PRIVATE /W4 /wd4996 /WX /MP)
    target_compile_options(relive_api_test PRIVATE /W4 /wd4996 /WX /MP)
endif()

target_include_directories(relive_api_test PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../3rdParty/magic_enum/include
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
target_compile_features(relive_api_test
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)
target_compile_definitions(relive_api_test PRIVATE "_CRT_SECURE_NO_WARNINGS")
target_link_libraries(relive_api_test relive_api jsonxx AliveLibAE AliveLibAO)

if (MSVC AND CMAKE_GENERATOR MATCHES "Visual Studio")
    if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        add_custom_command(TARGET relive_api_test POST_BUILD
            COMMAND echo copy "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" "$(OutDir)SDL2.dll"
            COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" "$(OutDir)SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" DESTINATION ".")
    else()
        add_custom_command(TARGET relive_api_test POST_BUILD
            COMMAND echo ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" "$(OutDir)SDL2.dll"
            COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" "$(OutDir)SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" DESTINATION ".")
    endif()
endif()
export(TARGETS relive_api_test FILE relive_api_test.cmake)
install(TARGETS relive_api_test DESTINATION "${BINPATH}")
