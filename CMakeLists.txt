cmake_minimum_required(VERSION 3.14)

project(thlib
    LANGUAGES C
    DESCRIPTION "Library for working with Touhou assets"
)

add_library(${PROJECT_NAME}
    src/dat.c
    src/pbg3.c
    src/pbg4.c
    src/bitstream.c
    src/lzss.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 17
    C_EXTENSIONS OFF
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -pedantic
)

include(GNUInstallDirs)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS thlib
    EXPORT thlibTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES include/thlib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT thlibTargets
    FILE thlibTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/thlib
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/thlibConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/thlib
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/thlibConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/thlib
)

