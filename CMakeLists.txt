cmake_minimum_required(VERSION 3.10.0)
project(InstrumentAudioVisualiser VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCE_FILES "src/*.cpp")

add_executable(
    InstrumentAudioVisualiser
    ${SOURCE_FILES}
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(InstrumentAudioVisualiser PRIVATE -stdlib=libc++)
    target_link_options(InstrumentAudioVisualiser PRIVATE -stdlib=libc++ -lc++abi)
endif()

target_include_directories(InstrumentAudioVisualiser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR/include})

