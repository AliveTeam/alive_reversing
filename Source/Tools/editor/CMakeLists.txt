cmake_minimum_required(VERSION 3.10)

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

find_package(QT NAMES Qt5 COMPONENTS Widgets Network LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network LinguistTools REQUIRED)

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
    ../../relive_lib/data_conversion/AutoFile.cpp
    ../../relive_lib/data_conversion/guid.cpp
    ../../relive_lib/data_conversion/relive_tlvs_serialization.cpp
    ../../relive_lib/data_conversion/CommonTypesSerialization.cpp
)
target_include_directories(editor_glue_dynamic_crt PRIVATE
    ../../relive_lib
)
target_link_libraries(editor_glue_dynamic_crt PUBLIC spng_dynamic_crt miniz_dynamic_crt SDL3::SDL3)

if (MSVC)
    target_compile_options(editor_glue_dynamic_crt PRIVATE /MDd)
endif()


add_library(relive_ipc_dynamic_crt
    ../../relive_lib/Ipc/Ipc.cpp
    ../../relive_lib/Ipc/Win32Ipc.cpp
    ../../relive_lib/Ipc/UnixIpc.cpp
)
target_include_directories(relive_ipc_dynamic_crt PRIVATE
    ../../relive_lib
)

if (MSVC)
    target_compile_options(relive_ipc_dynamic_crt PRIVATE /MDd)
endif()

set(PROJECT_SOURCES
        Source/GridSnapSettings.hpp
        Source/IGridPointSnapper.hpp
        Source/ClipBoard.cpp
        Source/ClipBoard.hpp
        Source/PasteItemsCommand.cpp
        Source/PasteItemsCommand.hpp
        Source/DeleteItemsCommand.cpp
        Source/DeleteItemsCommand.hpp
        Source/SetSelectionCommand.cpp
        Source/SetSelectionCommand.hpp
        Source/MoveItemsCommand.cpp
        Source/MoveItemsCommand.hpp
        Source/AddCollisionCommand.cpp
        Source/AddCollisionCommand.hpp
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
        Source/EditorMod.hpp
        Source/EditorMod.cpp
        Source/ModTreeItem.hpp
        Source/ModTreeItem.cpp
        Source/ModTreeWidget.hpp
        Source/ModTreeWidget.cpp
        Source/NewModDialog.hpp
        Source/NewModDialog.cpp
        Source/NewModDialog.ui
        Source/CollisionConnect.cpp
        Source/CollisionConnect.hpp
        Source/IGraphicsItem.hpp
        Source/main.cpp
        Source/AutomationProtocol.hpp
        Source/AutomationServer.hpp
        Source/AutomationServer.cpp
        Source/AutomationCommands.hpp
        Source/AutomationCommands.cpp
        Source/AutomationSceneCommands.hpp
        Source/AutomationSceneCommands.cpp
        Source/AboutDialog.ui
        Source/EditorMainWindow.ui
        Source/EditorTab.ui
        Source/EditorMainWindow.cpp
        Source/EditorMainWindow.hpp
        Source/EditorTab.cpp
        Source/EditorTab.hpp
        Source/EditorGraphicsView.cpp
        Source/EditorGraphicsView.hpp
        Source/FireWidget.cpp
        Source/FireWidget.hpp
        Source/AboutDialog.cpp
        Source/AboutDialog.hpp
        Source/Model.cpp
        Source/Model.hpp
        Source/EditorCamera.hpp
        Source/CollisionObject.cpp
        Source/CollisionObject.hpp
        Source/ResizeableArrowItem.cpp
        Source/ResizeableArrowItem.hpp
        Source/ResizeableRectItem.cpp
        Source/ResizeableRectItem.hpp
        Source/CameraGraphicsItem.cpp
        Source/CameraGraphicsItem.hpp
        Source/EditorGraphicsScene.cpp
        Source/EditorGraphicsScene.hpp
        Source/ItemPositionData.cpp
        Source/ItemPositionData.hpp
        Source/TransparencySettings.hpp
        Source/BigSpinBox.cpp
        Source/BigSpinBox.hpp
        Source/PropertyTreeItemBase.hpp
        Source/ISyncPropertiesToTree.hpp
        Source/StringProperty.hpp
        Source/StringProperty.cpp
        Source/ChangeStringPropertyCommand.hpp
        Source/ChangeStringPropertyCommand.cpp
        Source/ReadOnlyStringProperty.hpp
        Source/ReadOnlyStringProperty.cpp
        Source/PropertyTreeWidget.cpp
        Source/PropertyTreeWidget.hpp
        Source/BasicTypeProperty.hpp
        Source/BasicTypeProperty.cpp
        Source/ChangeBasicTypePropertyCommand.hpp
        Source/ChangeBasicTypePropertyCommand.cpp
        Source/IntegerType.hpp
        Source/BasicTypePropertyChangeData.hpp
        Source/LinkedBasicTypeProperty.hpp
        Source/BoolProperty.hpp
        Source/BoolProperty.cpp
        Source/ChangeBoolPropertyCommand.hpp
        Source/ChangeBoolPropertyCommand.cpp
        Source/BoolPropertyChangeData.hpp
        Source/LinkedBoolProperty.hpp
        Source/EnumProperty.cpp
        Source/EnumProperty.hpp
        Source/EnumPropertyBase.hpp
        Source/EnumPropertyChangeData.hpp
        Source/LinkedEnumProperty.hpp
        Source/ChangeEnumPropertyCommand.hpp
        Source/ReflectedEnumProperties.hpp
        Source/ColourPickerProperty.cpp
        Source/ColourPickerProperty.hpp
        Source/ChangeColourPickerPropertyCommand.hpp
        Source/ChangeColourPickerPropertyCommand.cpp
        Source/resources.qrc
        ${TS_FILES}
)

#if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#    qt_create_translation(QM_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source" ${TS_FILES} OPTIONS -no-obsolete -no-ui-lines -locations none)
#    qt_add_executable(relive-editor ${PROJECT_SOURCES} ${QM_FILES})
#else()
    qt5_create_translation(QM_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source" ${TS_FILES} OPTIONS -no-obsolete -no-ui-lines -locations none)
    if(WIN32)
        set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/Source/rsc/icons/relive-editor.rc")
        add_executable(relive-editor ${PROJECT_SOURCES} ${QM_FILES} ${APP_ICON_RESOURCE_WINDOWS})
    else()
        add_executable(relive-editor ${PROJECT_SOURCES} ${QM_FILES})
    endif()
#endif()

if (UNIX)
    SET(INSTAL_BINPATH "${CMAKE_BINARY_DIR}/CommonAppDir")
    install(TARGETS relive-editor DESTINATION "${INSTAL_BINPATH}/ReliveEditor/usr/bin")
endif()

if (MSVC)
    target_compile_options(relive-editor PRIVATE /MDd)
    # Qt 5.15 headers use stdext::checked_array_iterator which newer MSVC STLs deprecate (STL4043)
    target_compile_definitions(relive-editor PRIVATE _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)
    # vorbisfile is built with the project wide static CRT, drop it in favour of the dynamic one (LNK4098)
    target_link_options(relive-editor PRIVATE /NODEFAULTLIB:$<IF:$<CONFIG:Debug>,LIBCMTD,LIBCMT>)
    set(MY_PATH "PATH=%PATH%;${Qt5_DIR}/../../../bin")
    set_target_properties(relive-editor PROPERTIES VS_DEBUGGER_ENVIRONMENT "${MY_PATH}")
endif()

target_link_libraries(relive-editor PUBLIC relive_api SDL3::SDL3 vorbisfile Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network editor_glue_dynamic_crt relive_ipc_dynamic_crt)

add_executable(relive-editor-automation-cli tools/AutomationCli.cpp Source/AutomationProtocol.hpp)
target_include_directories(relive-editor-automation-cli PRIVATE Source)
target_link_libraries(relive-editor-automation-cli PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)
if (MSVC)
    target_compile_options(relive-editor-automation-cli PRIVATE /MDd)
endif()

add_executable(EditorAutomationTests tests/EditorAutomationTests.cpp Source/AutomationProtocol.hpp)
target_include_directories(EditorAutomationTests PRIVATE Source)
target_link_libraries(EditorAutomationTests PUBLIC googletest_dynamic_crt Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Gui)
target_compile_definitions(EditorAutomationTests PRIVATE RELIVE_EDITOR_PATH="$<TARGET_FILE:relive-editor>")
if (MSVC)
    target_compile_options(EditorAutomationTests PRIVATE /MDd)
endif()
add_test(NAME EditorAutomationTests COMMAND EditorAutomationTests)

# Plain unit tests for pure placement math (GridPlacement.hpp) - no Qt/GUI dependency at all,
# unlike EditorAutomationTests above which drives the real editor process end-to-end.
add_executable(GridPlacementTests tests/GridPlacementTests.cpp Source/GridPlacement.hpp)
target_include_directories(GridPlacementTests PRIVATE Source)
target_link_libraries(GridPlacementTests PUBLIC googletest_dynamic_crt)
if (MSVC)
    target_compile_options(GridPlacementTests PRIVATE /MDd)
endif()
add_test(NAME GridPlacementTests COMMAND GridPlacementTests)