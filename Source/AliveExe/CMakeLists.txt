if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

add_executable(AliveExe WIN32 
    Exe.cpp 
    resource.rc
    resource.h
    app.ico)
target_include_directories(AliveExe PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
target_compile_features(AliveExe
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)
target_compile_definitions(AliveExe PRIVATE "_CRT_SECURE_NO_WARNINGS")
target_link_libraries(AliveExe AliveLibAE AliveLibAO)
export(TARGETS AliveExe FILE AliveExe.cmake)
install(TARGETS AliveExe DESTINATION "${BINPATH}")

add_executable(AliveExeAE WIN32 
    Exe.cpp 
    resource.rc
    resource.h
    app.ico)
target_include_directories(AliveExeAE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
target_compile_features(AliveExeAE
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)
target_compile_definitions(AliveExeAE PRIVATE "_CRT_SECURE_NO_WARNINGS" "AE_EXE")
target_link_libraries(AliveExeAE AliveLibAE AliveLibAO)
export(TARGETS AliveExeAE FILE AliveExeAE.cmake)
install(TARGETS AliveExeAE DESTINATION "${BINPATH}")


add_executable(AliveExeAO WIN32 
    Exe.cpp 
    resource.rc
    resource.h
    app.ico)
target_include_directories(AliveExeAO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
target_compile_features(AliveExeAO
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)
target_compile_definitions(AliveExeAO PRIVATE "_CRT_SECURE_NO_WARNINGS" "AO_EXE")
target_link_libraries(AliveExeAO AliveLibAE AliveLibAO)
export(TARGETS AliveExeAO FILE AliveExeAO.cmake)
install(TARGETS AliveExeAO DESTINATION "${BINPATH}")
