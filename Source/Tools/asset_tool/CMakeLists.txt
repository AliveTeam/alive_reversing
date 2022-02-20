if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    add_executable(asset_tool WIN32
    asset_tool.cpp
	resource.rc
    resource.h
    asset_common.cpp
    asset_common.hpp
    asset_meta.cpp
    asset_meta.hpp
    asset_video.hpp
    )

    if (MSVC)
        target_compile_options(asset_tool PRIVATE /W4 /wd4996 /WX /MP)
    endif()

    target_include_directories(asset_tool PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        PRIVATE ${CMAKE_BINARY_DIR})
    target_compile_features(asset_tool
        PRIVATE cxx_auto_type
        PRIVATE cxx_variadic_templates)
    target_compile_definitions(asset_tool PRIVATE "_CRT_SECURE_NO_WARNINGS")
    target_link_libraries(asset_tool AliveLibAE AliveLibAO imgui libglew_static jsonxx_reliveapi)

    export(TARGETS asset_tool FILE asset_tool.cmake)
    install(TARGETS asset_tool DESTINATION "${BINPATH}")
endif()
