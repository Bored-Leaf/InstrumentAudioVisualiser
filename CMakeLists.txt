cmake_minimum_required(VERSION 3.10.0)
project(InstrumentAudioVisualiser VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OpenGL_GL_PREFERENCE "GLVND")

find_package(glfw3 3.5 REQUIRED)
find_package(OpenGL REQUIRED)

# Copies shader files to build directory
file(GLOB SHADER_FILES CONFIGURE_DEPENDS "src/shaders/*")
add_custom_target(CopyShaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${SHADER_FILES} ${CMAKE_BINARY_DIR}/shaders
    COMMENT "Copying shaders to build directory..."
)

file(GLOB SOURCE_FILES "src/*.cpp" "src/*.c" "learning/*.cpp")

add_executable(
    InstrumentAudioVisualiser
    ${SOURCE_FILES}
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(InstrumentAudioVisualiser PRIVATE -stdlib=libc++)
    target_link_options(InstrumentAudioVisualiser PRIVATE -stdlib=libc++ -lc++abi)
endif()

target_include_directories(InstrumentAudioVisualiser PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/learning 
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm
)

target_link_libraries(InstrumentAudioVisualiser glfw OpenGL::GL)
