if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(relive_api_src
    ApiContext.hpp
    RoundUp.hpp
    file_api.hpp
    ApiFG1Reader.hpp
    ApiFG1Reader.cpp
    Base64.hpp
    Base64.cpp
    CamConverter.hpp
    CamConverter.cpp
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
    JsonUpgrader.cpp
    JsonUpgrader.hpp
    JsonUpgraderMacros.hpp
    LvlReaderWriter.hpp
    PropertyCollection.cpp
    PropertyCollection.hpp
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
target_link_libraries(relive_api jsonxx)
set_property(TARGET relive_api PROPERTY FOLDER "ReliveAPI")
export(TARGETS relive_api FILE relive_api.cmake)

target_include_directories(relive_api PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/EasyLogging++/EasyLogging/src
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/googletest/googlemock/include
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/googletest/googletest/include
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/json/include
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
