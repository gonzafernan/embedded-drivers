cmake_minimum_required(VERSION 3.10)
project(embedded_drivers C CXX)

enable_testing()

add_library(icm20948 STATIC
    src/icm20948_driver.c
)

add_library(ak09916 STATIC
    src/ak09916_driver.c
)

target_include_directories(icm20948 PUBLIC include)
target_include_directories(ak09916 PUBLIC include)

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(test_driver
    test/test_icm20948_driver.cpp
    test/test_ak09916_driver.cpp
    test/mock_i2c_port.c
)

target_link_libraries(test_driver
    icm20948
    ak09916
    gtest_main
)

target_include_directories(test_driver PRIVATE include test)

include(GoogleTest)
gtest_discover_tests(test_driver)