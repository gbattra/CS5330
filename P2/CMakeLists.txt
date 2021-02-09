cmake_minimum_required(VERSION 2.8)
project( Project2 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(
    ImgSearch imgSearch.cpp
    dbReader.h dbReader.cpp
    imgFeatures.h imgFeatures.cpp
    imgMetrics.h imgMetrics.cpp)
target_link_libraries( ImgSearch ${OpenCV_LIBS} )