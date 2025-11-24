cmake_minimum_required(VERSION 3.10.0)
project(InstrumentAudioVisualiser VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

file(GLOB SOURCE_FILES "src/*.cpp")

add_executable(
    InstrumentAudioVisualiser
    ${SOURCE_FILES}
)

target_include_directories(InstrumentAudioVisualiser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR/include})

