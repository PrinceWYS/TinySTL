include_directories(${PROJECT_SOURCE_DIR}/include)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(test_allocator test_allocator.cpp)
add_executable(test_alloc test_alloc.cpp)