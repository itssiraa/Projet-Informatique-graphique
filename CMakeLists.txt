cmake_minimum_required(VERSION 3.10)

project(OpenGLProgram)

# Set directories
set(SRC_DIR src)
set(INCLUDE_DIR include)
set(EXTERNAL_DIR external)
set(GLEW_DIR ${EXTERNAL_DIR}/glew-cmake)
set(GLFW_DIR ${EXTERNAL_DIR}/glfw)
set(GLM_DIR ${EXTERNAL_DIR}/glm)

# Include directories
include_directories(${INCLUDE_DIR})
include_directories(${GLEW_DIR}/include)
include_directories(${GLFW_DIR}/include)
include_directories(${GLM_DIR})




# Source files
set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/node.cpp
    ${SRC_DIR}/shape.cpp
    ${SRC_DIR}/shader.cpp
    ${SRC_DIR}/viewer.cpp
    ${SRC_DIR}/triangle.cpp
    ${SRC_DIR}/pyramid.cpp
    ${SRC_DIR}/cylinder.cpp
    ${SRC_DIR}/sphere.cpp
    ${SRC_DIR}/texture.cpp
    ${SRC_DIR}/textured_sphere.cpp
    ${SRC_DIR}/lighting_sphere.cpp
    ${SRC_DIR}/demi_sphere.cpp
    ${SRC_DIR}/BoatAnimator.cpp
    ${SRC_DIR}/bords.cpp
    ${SRC_DIR}/OrbitCamera.cpp
    ${SRC_DIR}/water.cpp
    ${SRC_DIR}/Program.cpp
    ${SRC_DIR}/ShaderUnit.cpp
    ${SRC_DIR}/TextureLoader.cpp


)

# Define Shader director
add_compile_definitions(SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders/")
add_compile_definitions(TEXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/textures/")

# GLEW library
add_subdirectory(${GLEW_DIR})
set(LIBS ${LIBS} libglew_static)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# GLFW library
add_subdirectory(${GLFW_DIR})
set(LIBS ${LIBS} glfw)

# GLM library
add_subdirectory(${GLM_DIR})

# Executable
add_executable(opengl_program ${SOURCES})
target_link_libraries(opengl_program ${LIBS})




file(GLOB SHADERS "${CMAKE_SOURCE_DIR}/shaders/*.frag" "${CMAKE_SOURCE_DIR}/shaders/*.vert")

foreach(shader_file ${SHADERS})
    configure_file(${shader_file} ${CMAKE_CURRENT_BINARY_DIR}/shaders/ COPYONLY)
endforeach()

file(GLOB WATER_SHADERS
    "${CMAKE_SOURCE_DIR}/shaders/water-*.vs"
    "${CMAKE_SOURCE_DIR}/shaders/water-*.fs"
    "${CMAKE_SOURCE_DIR}/shaders/water-*.glsl"
)

foreach(shader_file ${WATER_SHADERS})
    configure_file(${shader_file} ${CMAKE_CURRENT_BINARY_DIR}/shaders/ COPYONLY)
endforeach()

# Copie tous les fichiers de textures (images .jpg et .png) + sous-dossiers
file(COPY ${CMAKE_SOURCE_DIR}/textures DESTINATION ${CMAKE_BINARY_DIR})
