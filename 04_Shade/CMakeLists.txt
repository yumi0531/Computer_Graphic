cmake_minimum_required(VERSION 3.14)

message("CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")

# Define project name and detail
project("glut_example"
    VERSION 0.1.0
    LANGUAGES C CXX
    DESCRIPTION "GLUT program example"
)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Define the executable name of this project, and create a CMake binary target
# !改這邊的exe檔
add_executable(lightExe "light.c")
add_executable(shadeExe "shade.c")
add_executable(windmillExe "wind_mill.c")

# !改這邊的link
# Assign source files to the executable.
target_link_libraries(lightExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
target_link_libraries(shadeExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
target_link_libraries(windmillExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)


if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(CarExe PRIVATE X11)
    target_link_libraries(CubeExe PRIVATE X11)
    target_link_libraries(TemplateExe PRIVATE X11)
    target_link_libraries(WindMillExe PRIVATE X11)
endif()