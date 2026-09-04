SET(INSTAL_BINPATH "${CMAKE_BINARY_DIR}/CommonAppDir")

if(UNIX AND NOT APPLE)
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/relive.png" DESTINATION ${INSTAL_BINPATH}/Relive)
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/relive.desktop" DESTINATION ${INSTAL_BINPATH}/Relive)
  install(FILES ReliveAppRun DESTINATION ${INSTAL_BINPATH}/Relive RENAME AppRun)

  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/relive-editor.png" DESTINATION ${INSTAL_BINPATH}/ReliveEditor)
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/relive-editor.desktop" DESTINATION ${INSTAL_BINPATH}/ReliveEditor)
  install(FILES EditorAppRun DESTINATION ${INSTAL_BINPATH}/ReliveEditor RENAME AppRun)
  endif()

set(APPIMAGE_TOOL "${CMAKE_BINARY_DIR}/linuxdeployqt")


add_custom_command(
    OUTPUT ${APPIMAGE_TOOL}
    COMMAND wget -O ${APPIMAGE_TOOL} https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
    COMMAND chmod +x ${APPIMAGE_TOOL}
    COMMENT "Downloading appimagetool..."
)
add_custom_target(appimage_tool DEPENDS ${APPIMAGE_TOOL})

add_custom_target(appimage
    DEPENDS appimage_tool
    COMMAND ${CMAKE_COMMAND} --install . --prefix "${INSTAL_BINPATH}"
#    COMMAND chmod +x "${INSTAL_BINPATH}/Relive/AppRun"
#    COMMAND chmod +x "${INSTAL_BINPATH}/Relive/usr/bin/relive"
#    COMMAND chmod +x "${INSTAL_BINPATH}/ReliveEditor/AppRun"
#    COMMAND chmod +x "${INSTAL_BINPATH}/ReliveEditor/usr/bin/relive-editor"
    COMMAND ${CMAKE_BINARY_DIR}/linuxdeployqt "${INSTAL_BINPATH}/Relive/usr/bin/relive" -unsupported-allow-new-glibc -bundle-non-qt-libs -appimage
    COMMAND ${CMAKE_BINARY_DIR}/linuxdeployqt "${INSTAL_BINPATH}/ReliveEditor/usr/bin/relive-editor" -unsupported-allow-new-glibc -bundle-non-qt-libs -appimage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building AppImage..."
)
