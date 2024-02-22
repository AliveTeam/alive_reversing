cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Multimedia LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Multimedia LinguistTools REQUIRED)

set(TS_FILES
  Source/rsc/translations/qt-editor_en_GB.ts
  Source/rsc/translations/qt-editor_de_DE.ts)

include_directories(../../../3rdParty/libmodplug/src)
include_directories(../../../3rdParty/EasyLogging++/EasyLogging/src)
include_directories(../../../3rdParty/json/single_include)

include_directories(Source)

add_library(editor_glue_dynamic_crt
    # Hack to include bits the editor uses, can't link the libs directly
    # due to diff CRT settings (static vs dynamic)
    ../../relive_lib/Grid.cpp
    ../../AliveLibAE/PathData.cpp
    ../../relive_lib/Grid.cpp
    ../../AliveLibAO/PathData.cpp
    ../../relive_lib/CompressionType_4Or5.cpp
    ../../relive_lib/CamDecompressor.cpp
    ../../relive_lib/AnimResources.cpp
    ../../relive_lib/MapWrapper.cpp
    ../../relive_lib/data_conversion/PNGFile.cpp
    ../../relive_lib/data_conversion/file_system.cpp
    ../../relive_lib/data_conversion/guid.cpp
)
target_include_directories(editor_glue_dynamic_crt PRIVATE
    ../../relive_lib
)
target_link_libraries(editor_glue_dynamic_crt PUBLIC spng_dynamic_crt miniz_dynamic_crt)

if (MSVC)
    target_compile_options(editor_glue_dynamic_crt PRIVATE /MDd)
endif()

set(PROJECT_SOURCES
        Source/ReliveApiWrapper.hpp
        Source/SnapSettings.hpp
        Source/ClipBoard.cpp
        Source/ClipBoard.hpp
        Source/DeleteItemsCommand.cpp
        Source/DeleteItemsCommand.hpp
        Source/ProgressDialog.hpp
        Source/ProgressDialog.cpp
        Source/ProgressDialog.ui
        Source/TransparencyDialog.hpp
        Source/TransparencyDialog.cpp
        Source/TransparencyDialog.ui
        Source/SelectionSaver.hpp
        Source/AddObjectDialog.hpp
        Source/AddObjectDialog.cpp
        Source/AddObjectDialog.ui
        Source/ChangeMapSizeDialog.hpp
        Source/ChangeMapSizeDialog.cpp
        Source/ChangeMapSizeDialog.ui
        Source/PathDataEditorDialog.hpp
        Source/PathDataEditorDialog.cpp
        Source/PathDataEditorDialog.ui
        Source/MessageEditorDialog.cpp
        Source/MessageEditorDialog.hpp
        Source/MessageEditorDialog.ui
        Source/CameraManager.cpp
        Source/CameraManager.hpp
        Source/CameraManager.ui
        Source/CollisionConnect.cpp
        Source/CollisionConnect.hpp
        Source/ExportPathDialog.hpp
        Source/ExportPathDialog.cpp
        Source/ExportPathDialog.ui
        Source/PathSelectionDialog.hpp
        Source/PathSelectionDialog.cpp
        Source/PathSelectionDialog.ui
        Source/IGraphicsItem.hpp
        Source/main.cpp
        Source/AboutDialog.ui
        Source/EditorMainWindow.ui
        Source/EditorTab.ui
        Source/EditorMainWindow.cpp
        Source/EditorMainWindow.hpp
        Source/EditorTab.cpp
        Source/EditorTab.hpp
        Source/FireWidget.cpp
        Source/FireWidget.hpp
        Source/AboutDialog.cpp
        Source/AboutDialog.hpp
        Source/ModThread.cpp
        Source/ModThread.hpp
        Source/Model.cpp
        Source/Model.hpp
        Source/ResizeableArrowItem.cpp
        Source/ResizeableArrowItem.hpp
        Source/ResizeableRectItem.cpp
        Source/ResizeableRectItem.hpp
        Source/CameraGraphicsItem.cpp
        Source/CameraGraphicsItem.hpp
        Source/EditorGraphicsScene.cpp
        Source/EditorGraphicsScene.hpp
        Source/BigSpinBox.cpp
        Source/BigSpinBox.hpp
        Source/PropertyTreeItemBase.hpp
        Source/StringProperty.hpp
        Source/StringProperty.cpp
        Source/PropertyTreeWidget.cpp
        Source/PropertyTreeWidget.hpp
        Source/BasicTypeProperty.hpp
        Source/BasicTypeProperty.cpp
        Source/BoolProperty.hpp
        Source/BoolProperty.cpp
        Source/EnumProperty.cpp
        Source/EnumProperty.hpp
        Source/ReflectedEnumProperties.hpp
        Source/ColourPickerProperty.cpp
        Source/ColourPickerProperty.hpp
        Source/resources.qrc
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_create_translation(QM_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source" ${TS_FILES} OPTIONS -no-obsolete -no-ui-lines -locations none)
    qt_add_executable(qt-editor ${PROJECT_SOURCES} ${QM_FILES})
else()
    qt5_create_translation(QM_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source" ${TS_FILES} OPTIONS -no-obsolete -no-ui-lines -locations none)
    if(ANDROID)
        add_library(qt-editor SHARED ${PROJECT_SOURCES} ${QM_FILES})
    elseif(WIN32)
        set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/Source/rsc/icons/qt-editor.rc")
        add_executable(qt-editor ${PROJECT_SOURCES} ${QM_FILES} ${APP_ICON_RESOURCE_WINDOWS})
    else()
        add_executable(qt-editor ${PROJECT_SOURCES} ${QM_FILES})
    endif()
endif()

if (MSVC)
    target_compile_options(qt-editor PRIVATE /MDd)
    set(MY_PATH "PATH=%PATH%;${QT_DIR}/../../../bin")
    set_target_properties(qt-editor PROPERTIES VS_DEBUGGER_ENVIRONMENT "${MY_PATH}")
endif()

target_link_libraries(qt-editor PUBLIC relive_api Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia modplug editor_glue_dynamic_crt)
