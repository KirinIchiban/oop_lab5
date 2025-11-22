cmake_minimum_required(VERSION 3.10)
project(lab5 LANGUAGES CXX)

add_compile_options(-Wall)    

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab5_exe main.cpp src/my_pmr.cpp)
target_include_directories(lab5_exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

#для тестов
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
  TLS_VERIFY false
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(my-tests test/tests5.cpp src/my_pmr.cpp) 
target_include_directories(my-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(my-tests
    GTest::gtest
    GTest::gtest_main
)
add_test(NAME GTests COMMAND my-tests)