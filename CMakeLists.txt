cmake_minimum_required(VERSION 3.12)
project(learnOpenGL)
set(CMAKE_CXX_STANDARD 14)
# OpenCV
find_package(opencv REQUIRED)
include_directories(${OPENCV_INCLUDE_DIR})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# GLUT
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

# GLFW + GLEW
set(GLEW_H /usr/local/Cellar/glew/2.1.0/include)
set(GLFW_H /usr/local/Cellar/glfw/3.2.1/include)
include_directories(${GLEW_H} ${GLFW_H})
set(GLEW_LINK /usr/local/Cellar/glew/2.1.0/lib/libGLEW.2.1.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.2.1/lib/libglfw.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

# freeimage
#set(FreeImage_H /usr/local/Cellar/freeimage/3.18.0/include/)
#include_directories(${FreeImage_H})
#set(FreeImage_LINK /usr/local/Cellar/freeimage/3.18.0/lib/freeimage.a)
#link_libraries(${FreeImage_LINK})

add_executable(hello_window hello_window/hello_window.cpp)
target_link_libraries(hello_window ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

add_executable(hello_triangle hello_triangle/hello_triangle.cpp)
target_link_libraries(hello_triangle ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

add_executable(hello_shader hello_shader/hello_shader.cpp)
target_link_libraries(hello_shader ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

add_executable(hello_texture hello_texture/hello_texture.cpp)
target_link_libraries(hello_texture ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${OpenCV_LIBS})

add_executable(hello_transform hello_transform/hello_transform.cpp)
target_link_libraries(hello_transform ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${OpenCV_LIBS})

add_executable(hello_coordinate hello_coordinate/hello_coordinate.cpp)
target_link_libraries(hello_coordinate ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${OpenCV_LIBS})

add_executable(hello_coordinate2 hello_coordinate/hello_coordinate2.cpp)
target_link_libraries(hello_coordinate2 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${OpenCV_LIBS})

add_executable(hello_camera hello_camera/hello_camera.cpp)
target_link_libraries(hello_camera ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${OpenCV_LIBS})

add_executable(hello_camera2 hello_camera/hello_camera2.cpp)
target_link_libraries(hello_camera2 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${OpenCV_LIBS})

add_executable(hello_color hello_color/hello_color.cpp)
target_link_libraries(hello_color ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${OpenCV_LIBS})

add_executable(hello_lighting hello_lighting/hello_lighting.cpp)
target_link_libraries(hello_lighting ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${OpenCV_LIBS})

add_executable(hello_lighting2 hello_lighting/hello_lighting2.cpp)
target_link_libraries(hello_lighting2 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${OpenCV_LIBS})

add_executable(read_mesh mesh/mesh.cpp)
target_link_libraries(read_mesh ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})