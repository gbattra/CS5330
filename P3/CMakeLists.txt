cmake_minimum_required(VERSION 2.8)
project( Project3 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( OR2D or2d.cpp )
target_link_libraries( OR2D ${OpenCV_LIBS} )