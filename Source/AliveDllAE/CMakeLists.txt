add_library(AliveDllAE SHARED dllmain.cpp)

target_include_directories(AliveDllAE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_compile_features(AliveDllAE
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

if (MSVC)
    set_target_properties(AliveDllAE PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /functionpadmin:16 /MAP")
    set_target_properties(AliveDllAE PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /functionpadmin:16 /MAP")
endif()

target_compile_definitions(AliveDllAE PRIVATE "_CRT_SECURE_NO_WARNINGS")

target_link_libraries(AliveDllAE AliveLibAE)
if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
   target_link_libraries(AliveDllAE Detours)
endif()

# Note: Copy AE dll to AO dir as the AO dll depends on it
if (WIN32)
    add_custom_command(TARGET AliveDllAE
                       POST_BUILD
                       COMMENT "Copying to game dirs ${AE_PATH} ${AO_PATH}"
                       COMMAND echo make "${CMAKE_COMMAND}" -E make_directory "${AE_PATH}"
                       COMMAND "${CMAKE_COMMAND}" -E make_directory "${AE_PATH}"
                       COMMAND echo make "${CMAKE_COMMAND}" -E make_directory "${AO_PATH}"
                       COMMAND "${CMAKE_COMMAND}" -E make_directory "${AO_PATH}"
                       COMMAND echo "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:AliveDllAE>" "${AE_PATH}/AliveDllAE.dll"
                       COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:AliveDllAE>" "${AE_PATH}/AliveDllAE.dll"
                       COMMAND echo "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:AliveDllAE>" "${AO_PATH}/AliveDllAE.dll"
                       COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:AliveDllAE>" "${AO_PATH}/AliveDllAE.dll"
                       COMMAND echo "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE_DIR:AliveDllAE>/AliveDllAE.map" "${AE_PATH}/AliveDllAE.map"
                       COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE_DIR:AliveDllAE>/AliveDllAE.map" "${AE_PATH}/AliveDllAE.map")
endif()

export(TARGETS AliveDllAE FILE AliveDllAE.cmake)
install(TARGETS AliveDllAE DESTINATION ".")

install(FILES "${AE_PATH}/AliveDllAE.map" DESTINATION ".")

if (WIN32)
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        add_custom_command(TARGET AliveDllAE POST_BUILD
            COMMAND echo copy "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" "$<TARGET_FILE_DIR:AliveDllAE>/SDL2.dll"
            COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" "$<TARGET_FILE_DIR:AliveDllAE>/SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")
    else()
        add_custom_command(TARGET AliveDllAE POST_BUILD
            COMMAND echo "${CMAKE_COMMAND}" -E copy "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" "$<TARGET_FILE_DIR:AliveDllAE>/SDL2.dll"
            COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" "$<TARGET_FILE_DIR:AliveDllAE>/SDL2.dll")
        INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/x64/SDL2.dll" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()
endif()
