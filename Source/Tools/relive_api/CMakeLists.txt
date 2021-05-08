if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(relive_api_src
    JsonReadUtils.hpp
    JsonModelTypes.hpp
    JsonModelTypes.cpp
    JsonMapRootInfoReader.hpp
    JsonMapRootInfoReader.cpp
    JsonReaderBase.hpp
    JsonReaderBase.cpp
    JsonReaderAE.hpp
    JsonReaderAE.cpp
    JsonReaderAO.hpp
    JsonReaderAO.cpp
    JsonWriterBase.hpp
    JsonWriterBase.cpp
    JsonWriterAE.hpp
    JsonWriterAE.cpp
    JsonWriterAO.hpp
    JsonWriterAO.cpp
    JsonUpgraderAE.cpp
    JsonUpgraderAE.hpp
    TlvsAE.hpp
    JsonUpgraderAO.cpp
    JsonUpgraderAO.hpp
    TlvsAO.hpp
    BaseProperty.cpp
    BaseProperty.hpp
    BasicType.hpp
    BasicTypeBase.cpp
    BasicTypeBase.hpp
    ByteStream.hpp
    EnumType.hpp
    EnumTypeBase.hpp
    ITypeBase.cpp
    ITypeBase.hpp
    JsonDocument.cpp
    JsonUpgrader.cpp
    JsonUpgrader.hpp
    JsonUpgraderMacros.hpp
    LvlReaderWriter.hpp
    PropertyCollection.cpp
    PropertyCollection.hpp
    relive_api_test.cpp
    relive_api.cpp
    relive_api.hpp
    relive_api_exceptions.hpp
    TlvFactory.cpp
    TlvFactory.hpp
    TlvObjectBase.cpp
    TlvObjectBase.hpp
    TlvObjectBaseAE.cpp
    TlvObjectBaseAE.hpp
    TlvObjectBaseAO.cpp
    TlvObjectBaseAO.hpp
    TlvObjectBaseMacros.hpp
    TypedProperty.cpp
    TypedProperty.hpp
    TypesCollectionAE.cpp
    TypesCollectionAE.hpp
    TypesCollectionAO.cpp
    TypesCollectionAO.hpp
    TypesCollectionBase.cpp
    TypesCollectionBase.hpp
)
add_library(relive_api ${relive_api_src})
target_link_libraries(relive_api jsonxx AliveLibAE AliveLibAO project_warnings)
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
target_link_libraries(relive_api_test relive_api jsonxx AliveLibAE AliveLibAO project_warnings)

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
