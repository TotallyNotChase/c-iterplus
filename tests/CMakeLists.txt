##################################################
# Configure target for building the testing executable

set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/impls.c
)

set(HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/common.h
  ${CMAKE_CURRENT_SOURCE_DIR}/impls.h
  ${CMAKE_CURRENT_SOURCE_DIR}/stdnames.h
  ${CMAKE_CURRENT_SOURCE_DIR}/sugar.h
)

# Add the main executable
add_executable(iterplus_test ${HEADERS} ${SOURCES})

# Link the iterators interface lib
target_link_libraries(iterplus_test ${LIBNAME})
