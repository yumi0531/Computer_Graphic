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
add_executable(CarExe "car.c")
add_executable(CubesExe "cubes.c")
add_executable(TemplateExe "template.c")
add_executable(WindMillExe "wind_mill.c")
add_executable(WindMillSampleExe "wind_mill_sample.c")
add_executable(SampleExe "sample.c")


# !改這邊的link
# Assign source files to the executable.
target_link_libraries(CarExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
target_link_libraries(CubesExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
target_link_libraries(TemplateExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
target_link_libraries(WindMillExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
target_link_libraries(WindMillSampleExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
target_link_libraries(SampleExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)

# target_link_libraries(BlendExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
# target_link_libraries(mouse_keyExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
# target_link_libraries(menuExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
# target_link_libraries(draw_2Exe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
# target_link_libraries(pix_rw_sample_2Exe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
# target_link_libraries(BingExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
# target_link_libraries(PanelExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
# target_link_libraries(BufferExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
# target_link_libraries(singleExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(CarExe PRIVATE X11)
    target_link_libraries(CubeExe PRIVATE X11)
    target_link_libraries(TemplateExe PRIVATE X11)
    target_link_libraries(WindMillExe PRIVATE X11)
endif()