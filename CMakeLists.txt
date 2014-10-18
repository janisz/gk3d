#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)
project(gk3d)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

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
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

#OpenGL Mathematics
include(ExternalProject)
ExternalProject_Add(
    glm
    PREFIX ${CMAKE_SOURCE_DIR}/libs/glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    )
ExternalProject_Get_Property(glm source_dir)
set(GlmIncludeDir ${source_dir}/glm)
include_directories(${GlmIncludeDir})

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)


# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${gk3d_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${gk3d_VERSION_MINOR}")
include(CPack)
