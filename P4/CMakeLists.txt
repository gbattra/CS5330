cmake_minimum_required(VERSION 3.1)

project( Project4 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(
    AR ar.cpp
)
target_link_libraries( AR ${OpenCV_LIBS} )