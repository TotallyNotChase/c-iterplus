cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(c-iterplus LANGUAGES C)

##################################################
# Configure some universal settings

# Set C language standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
# Disable non standard extensions
set(CMAKE_C_EXTENSIONS OFF)

# Don't skimp on warnings
if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()
if(WIN32)
    # But ignore the Microsoft "Use *_s functions" warning
    add_compile_options(-Wno-deprecated-declarations)
endif()

##################################################
# Configure target for building the library

# Set library name
set(LIBNAME iterplus)

# Add the interface library target
add_library(${LIBNAME} INTERFACE)

# Add the headers to the library interface
target_include_directories(${LIBNAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(tests)
