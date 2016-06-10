# CMake settings.
cmake_minimum_required(VERSION 3.0.2)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Project details.
project(space-cowboy)
set(TARGET space_cowboy)

# Check compiler and pass compiler flags.
if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    set(OPTIMIZATION -O3)
    set(STANDARD -std=c++14)
    set(WARNINGS -Weverything)
    set(WARNINGS_OMIT "-Wno-c++98-compat \
                       -Wno-c++98-compat-pedantic \
                       -Wno-global-constructors \
                       -Wno-exit-time-destructors \
                       -Wno-missing-braces \
                       -Wno-padded")
    set(FLAGS "${OPTIMIZATION} ${STANDARD} ${WARNINGS} ${WARNINGS_OMIT}")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set(OPTIMIZATION -O3)
    set(STANDARD -std=c++14)
    set(WARNINGS "-Wall \
                  -Wextra \
                  -pedantic \
                  -Wcast-align \
                  -Wcast-qual \
                  -Wctor-dtor-privacy \
                  -Wdisabled-optimization \
                  -Wformat=2 \
                  -Wlogical-op \
                  -Wmissing-declarations \
                  -Wmissing-include-dirs \
                  -Wold-style-cast \
                  -Woverloaded-virtual \
                  -Wredundant-decls \
                  -Wshadow \
                  -Wsign-conversion \
                  -Wsign-promo \
                  -Wstrict-null-sentinel \
                  -Wswitch-default \
                  -Wundef")
    set(FLAGS "${OPTIMIZATION} ${STANDARD} ${WARNINGS}")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_definitions(-D_USE_MATH_DEFINES)
    set(FLAGS "/Za")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)
else ()
    message(FATAL_ERROR "Compiler is not supported by this project.")
endif ()

# Source files.
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[c,h]pp")

# Create executable.
add_executable(${TARGET} ${SOURCE_FILES})
set_target_properties(${TARGET} PROPERTIES
        COMPILE_FLAGS "${FLAGS}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        LINKER_LANGUAGE CXX)

# Link libraries.
if (UNIX)
    target_link_libraries(${TARGET} GL GLEW glfw SOIL)
elseif (WIN32)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)
    target_link_libraries(${TARGET}
            opengl32.lib
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/GL/glew32.lib
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/GLFW/glfw3dll.lib)
endif ()

# Custom targets.
add_custom_target(doc
        DEPENDS "${SOURCE_FILES}"
        COMMENT "Generating HTML and LaTeX documentation from Doxygen comments."
        COMMAND cd ../doc && doxygen space_cowboy.doxyfile)
