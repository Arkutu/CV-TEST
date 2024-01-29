cmake_minimum_required(VERSION 3.0.0)
project(CVTEST VERSION 0.1.0)





find_package(OpenCV REQUIRED)
find_package(dlib REQUIRED)


include(CTest)
enable_testing()

add_executable(CVTEST SCD.cpp)

target_link_libraries(CVTEST ${OpenCV_LIBS} dlib::dlib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
