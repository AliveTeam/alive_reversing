if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

add_executable(vab_tool WIN32 
    vab_tool.cpp)

target_include_directories(vab_tool PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
target_compile_features(vab_tool
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)
target_compile_definitions(vab_tool PRIVATE "_CRT_SECURE_NO_WARNINGS")
target_link_libraries(vab_tool AliveLibAE AliveLibAO)
export(TARGETS vab_tool FILE vab_tool.cmake)
install(TARGETS vab_tool DESTINATION "${BINPATH}")
