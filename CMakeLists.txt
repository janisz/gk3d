#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.8.12)
include(ExternalProject)
project(gk3d)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Set version information in a config.h file
set(gk3d_VERSION_MAJOR 0)
set(gk3d_VERSION_MINOR 1)
include_directories("${PROJECT_BINARY_DIR}")

# Define sources, libs and executable
set(EXECUTABLE_NAME "gk3d.out")
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/src/*.h")
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# OpenGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} )
target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY} )

#OpenGL Mathematics
ExternalProject_Add(
    glm
    PREFIX ${CMAKE_SOURCE_DIR}/libs/glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.5.4
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    )
ExternalProject_Get_Property(glm source_dir)
set(GlmIncludeDir ${source_dir}/glm)
include_directories(${GlmIncludeDir})
add_dependencies(${EXECUTABLE_NAME} glm)

# TinuObjLoader
ExternalProject_Add(
    tinyobjloader
    PREFIX ${CMAKE_SOURCE_DIR}/libs/tinyobjloader
    GIT_REPOSITORY https://github.com/syoyo/tinyobjloader.git
    GIT_TAG b35f4989ad90ef5f05e305c85923379882e4f9f0
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_SOURCE_DIR}/libs/tinyobjloader
    LOG_DOWNLOAD ON
    )
#TODO: Make it better
set(tinyobjloader ${CMAKE_SOURCE_DIR}/libs/tinyobjloader)
include_directories(${tinyobjloader}/include)
target_link_libraries(${EXECUTABLE_NAME} ${tinyobjloader}/lib/libtinyobjloader.a)
add_dependencies(${EXECUTABLE_NAME} tinyobjloader)

#Simple OpenGL Image Library
ExternalProject_Add(
    soil
    PREFIX ${CMAKE_SOURCE_DIR}/libs/soil
    GIT_REPOSITORY https://github.com/kbranigan/Simple-OpenGL-Image-Library.git
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_SOURCE_DIR}/libs/soil
    LOG_DOWNLOAD ON
    )
set(tinyobjloader ${CMAKE_SOURCE_DIR}/libs/soil)
include_directories(${tinyobjloader}/include)
target_link_libraries(${EXECUTABLE_NAME} ${tinyobjloader}/lib/libSOIL.a)
add_dependencies(${EXECUTABLE_NAME} soil)

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)


# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${gk3d_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${gk3d_VERSION_MINOR}")
include(CPack)
