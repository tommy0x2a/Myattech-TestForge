# out/CMakeLists.txt
# TestForge Stress Test Validation - CMake project
cmake_minimum_required(VERSION 3.15)
project(TestForge_StressTests C)

set(CMAKE_C_STANDARD 99)

# Unity framework (adjust path if needed)
set(UNITY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../tests/unity/src")

if(NOT EXISTS "${UNITY_DIR}/unity.h")
    message(FATAL_ERROR "Unity not found. Run 'testforge init' from project root first.")
endif()

add_library(unity STATIC ${UNITY_DIR}/unity.c)
target_include_directories(unity PUBLIC ${UNITY_DIR})

# Find all generated stress test files (they live under out/test_data/...)
file(GLOB_RECURSE GENERATED_TESTS
    "${CMAKE_CURRENT_SOURCE_DIR}/test_data/StressTestApp/**/*_test.c"
)

if(NOT GENERATED_TESTS)
    message(WARNING "No generated test files found yet. Run the validation script first.")
endif()

add_executable(stress_tests ${GENERATED_TESTS})

target_include_directories(stress_tests PRIVATE
    ${UNITY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/test_data/StressTestApp
)

target_link_libraries(stress_tests PRIVATE unity)

enable_testing()
add_test(NAME StressTestSuite COMMAND stress_tests)