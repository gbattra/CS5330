cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

project( Project3 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(
    OR2D or2d.cpp
    controller.h controller.cpp videoController.cpp imgController.cpp
    pipeline.h init.cpp threshold.cpp segment.cpp feature.cpp label.cpp
    features.h moments.cpp boundingBox.cpp regionFeatures.cpp)
target_link_libraries( OR2D ${OpenCV_LIBS} )