cmake_minimum_required(VERSION 2.8)
project( Project1 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} . )

add_executable( ImgDisplay imgDisplay.cpp filter.h filter.cpp )
target_link_libraries( ImgDisplay ${OpenCV_LIBS} )

add_executable( VidDisplay vidDisplay.cpp filter.h filter.cpp )
target_link_libraries( VidDisplay ${OpenCV_LIBS} )