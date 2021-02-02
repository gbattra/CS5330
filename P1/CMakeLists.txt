cmake_minimum_required(VERSION 2.8)
project( Project1 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( ImgDisplay imgDisplay.cpp filters.h filters.cpp )
target_link_libraries( ImgDisplay ${OpenCV_LIBS} )

add_executable( VidDisplay vidDisplay.cpp filters.h filters.cpp )
target_link_libraries( VidDisplay ${OpenCV_LIBS} )