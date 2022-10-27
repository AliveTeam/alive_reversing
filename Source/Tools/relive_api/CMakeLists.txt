if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# jsonxx lib, ditto defining here for relive_api upstream editor projects
add_library(jsonxx_reliveapi ../../../3rdParty/jsonxx/jsonxx.cc)
target_include_directories(jsonxx_reliveapi SYSTEM PUBLIC
    $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdParty
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdParty/json/single_include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdParty/jsonxx>)
export(TARGETS jsonxx_reliveapi FILE jsonxx_reliveapi.cmake)
set_property(TARGET jsonxx_reliveapi PROPERTY FOLDER "ReliveAPI")

add_library(easylogging_reliveapi
    ../../../3rdParty/EasyLogging++/EasyLogging/src/easylogging++.cc
    )
target_include_directories(easylogging_reliveapi PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/EasyLogging++/EasyLogging/src
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
export(TARGETS easylogging_reliveapi FILE easylogging_reliveapi.cmake)
set_property(TARGET easylogging_reliveapi PROPERTY FOLDER "ReliveAPI")

# relive_lib entry HACK! fix later after engine merge is finished
add_library(AliveLibAO_reliveapi
    ../../AliveLibAO/Grid.cpp
    ../../AliveLibAO/PathData.cpp
    ../../relive_lib/MapWrapper.cpp
    )
target_include_directories(AliveLibAO_reliveapi PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdParty/json/single_include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/EasyLogging++/EasyLogging/src
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
export(TARGETS AliveLibAO_reliveapi FILE AliveLibAO_reliveapi.cmake)
set_property(TARGET AliveLibAO_reliveapi PROPERTY FOLDER "ReliveAPI")

add_library(AliveLibAE_reliveapi
    ../../AliveLibAE/Grid.cpp
    ../../AliveLibAE/PathData.cpp
    )
target_include_directories(AliveLibAE_reliveapi PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/EasyLogging++/EasyLogging/src
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
export(TARGETS AliveLibAE_reliveapi FILE AliveLibAE_reliveapi.cmake)
set_property(TARGET AliveLibAE_reliveapi PROPERTY FOLDER "ReliveAPI")

add_library(AliveLibCommon_reliveapi
    ../../AliveLibCommon/AnimResources.cpp
    ../../AliveLibCommon/CamDecompressor.cpp
    ../../AliveLibCommon/CompressionType_4Or5.cpp
    )
target_include_directories(AliveLibCommon_reliveapi PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/EasyLogging++/EasyLogging/src
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
export(TARGETS AliveLibCommon_reliveapi FILE AliveLibCommon_reliveapi.cmake)
set_property(TARGET AliveLibCommon_reliveapi PROPERTY FOLDER "ReliveAPI")

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
target_link_libraries(relive_api 
    easylogging_reliveapi
    AliveLibCommon_reliveapi
    AliveLibAO_reliveapi
    AliveLibAE_reliveapi
    jsonxx_reliveapi)
set_property(TARGET relive_api PROPERTY FOLDER "ReliveAPI")
export(TARGETS relive_api FILE relive_api.cmake)

target_include_directories(relive_api PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/EasyLogging++/EasyLogging/src
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/googletest/googlemock/include
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/googletest/googletest/include
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
