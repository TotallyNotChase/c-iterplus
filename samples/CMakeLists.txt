##################################################
# Configure target for building the testing executable

set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/impls.c
  ${CMAKE_CURRENT_SOURCE_DIR}/sugar.c
)

set(HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/impls.h
  ${CMAKE_CURRENT_SOURCE_DIR}/sugar.h
)

set(EXCNAME iterplus_samples)

# Add the main executable
add_executable(${EXCNAME} ${HEADERS} ${SOURCES})

# Link the iterators interface lib
target_link_libraries(${EXCNAME} ${LIBNAME})

# Set C language standard to C11 (for `_Generic`)
# NOTE: The iterplus library works for C99 (and above), but the examples use C11 for convenience
set_property(TARGET ${EXCNAME} PROPERTY C_STANDARD 11)
set_property(TARGET ${EXCNAME} PROPERTY C_STANDARD_REQUIRED ON)
