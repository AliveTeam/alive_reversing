if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# lodepng lib, defined here as its the only project that uses it and so that
# users of relive_api don't need to build the whole of relive.
add_library(lodepng_reliveapi
    ../../../3rdParty/lodepng/lodepng.cpp
    ../../../3rdParty/lodepng/lodepng.h
    )

target_include_directories(lodepng_reliveapi SYSTEM PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdParty>
    $<INSTALL_INTERFACE:lodepng>)

set_property(TARGET lodepng_reliveapi PROPERTY FOLDER "3rdParty")
export(TARGETS lodepng_reliveapi FILE lodepng_reliveapi.cmake)

# jsonxx lib, ditto defining here for relive_api upstream editor projects
add_library(jsonxx_reliveapi ../../../3rdParty/jsonxx/jsonxx.cc)
target_include_directories(jsonxx_reliveapi SYSTEM PUBLIC
    $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdParty/jsonxx>)
export(TARGETS jsonxx_reliveapi FILE jsonxx_reliveapi.cmake)
set_property(TARGET jsonxx_reliveapi PROPERTY FOLDER "3rdParty")

add_library(easylogging_reliveapi
    ../../../3rdParty/EasyLogging++/EasyLogging/src/easylogging++.cc
    )
target_include_directories(easylogging_reliveapi PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/EasyLogging++/EasyLogging/src
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
export(TARGETS easylogging_reliveapi FILE easylogging_reliveapi.cmake)

add_library(AliveLibAO_reliveapi
    ../../AliveLibAO/PathData.cpp
    )
target_include_directories(AliveLibAO_reliveapi PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/EasyLogging++/EasyLogging/src
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
export(TARGETS AliveLibAO_reliveapi FILE AliveLibAO_reliveapi.cmake)

add_library(AliveLibAE_reliveapi
    ../../AliveLibAE/PathData.cpp
    )
target_include_directories(AliveLibAE_reliveapi PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/EasyLogging++/EasyLogging/src
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
export(TARGETS AliveLibAE_reliveapi FILE AliveLibAE_reliveapi.cmake)

add_library(AliveLibCommon_reliveapi
    ../../AliveLibCommon/CamDecompressor.cpp
    )
export(TARGETS AliveLibCommon_reliveapi FILE AliveLibCommon_reliveapi.cmake)

set(relive_api_src
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
target_link_libraries(relive_api 
    jsonxx_reliveapi
    lodepng_reliveapi
    easylogging_reliveapi
    AliveLibCommon_reliveapi
    AliveLibAO_reliveapi
    AliveLibAE_reliveapi)

export(TARGETS relive_api FILE relive_api.cmake)

target_include_directories(relive_api PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/EasyLogging++/EasyLogging/src
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/googletest/googlemock/include
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/googletest/googletest/include
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
