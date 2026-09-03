
get_target_property(SDL3_LIB SDL3::SDL3 LOCATION)
message("SDL3 .so is at: ${SDL3_LIB}")

if(UNIX AND NOT APPLE)
  SET(BINPATH "${CMAKE_BINARY_DIR}/AppDir")
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/relive.png" DESTINATION .)
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/relive.desktop" DESTINATION .)
  install(FILES AppRun DESTINATION .)
  install(FILES ${SDL3_LIB} DESTINATION ./usr/lib)
  endif()
set(APPDIR "${CMAKE_BINARY_DIR}/AppDir")

set(APPIMAGE_TOOL "${CMAKE_BINARY_DIR}/appimagetool")


add_custom_command(
    OUTPUT ${APPIMAGE_TOOL}
    COMMAND wget -O ${APPIMAGE_TOOL} https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
    COMMAND chmod +x ${APPIMAGE_TOOL}
    COMMENT "Downloading appimagetool..."
)
add_custom_target(appimage_tool DEPENDS ${APPIMAGE_TOOL})

add_custom_target(appimage
    DEPENDS appimage_tool
    COMMAND ${CMAKE_COMMAND} --install . --prefix "${APPDIR}"
    COMMAND chmod +x "${APPDIR}/AppRun"
    COMMAND chmod +x "${APPDIR}/usr/bin/relive"
    COMMAND ${CMAKE_BINARY_DIR}/appimagetool "${APPDIR}"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building AppImage..."
)
