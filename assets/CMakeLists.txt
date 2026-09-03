
if(UNIX AND NOT APPLE)
  SET(BINPATH "${CMAKE_BINARY_DIR}/AppDir")
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/relive.png" DESTINATION .)
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/relive.desktop" DESTINATION .)
  install(FILES AppRun DESTINATION .)
  endif()
set(APPDIR "${CMAKE_BINARY_DIR}/AppDir")

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
    COMMAND ${CMAKE_COMMAND} --install . --prefix "${APPDIR}"
    COMMAND chmod +x "${APPDIR}/AppRun"
    COMMAND chmod +x "${APPDIR}/usr/bin/relive"
#    COMMAND ${CMAKE_BINARY_DIR}/linuxdeployqt "${APPDIR}/usr/bin/relive" -unsupported-allow-new-glibc -bundle-non-qt-libs -appimage
    COMMAND ${CMAKE_BINARY_DIR}/linuxdeployqt "${APPDIR}/usr/bin/relive-editor" -unsupported-allow-new-glibc -bundle-non-qt-libs -appimage

    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building AppImage..."
)
