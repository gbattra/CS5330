cmake_minimum_required(VERSION 3.1)

project( Project4 )
find_package( OpenCV REQUIRED )
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${Project4_SOURCE_DIR})

add_executable(
    AR ar.cpp
    commands/commands.h commands/drawCornersCmd.cpp commands/registerFrameCmd.cpp
    commands/calibrateCmd.cpp commands/saveCalibrationCmd.cpp
    commands/loadCalibrationCmd.cpp commands/solvePnPCmd.cpp
    commands/projectObjectCmd.cpp

    controllers/controllers.h controllers/calibrationController.cpp
    controllers/projectionController.cpp

    models/models.h models/calibrator.cpp models/projector.cpp

    views/views.h views/cornersView.cpp views/calibrationView.cpp views/poseView.cpp
    views/objectView.cpp

    utils/utils.h utils/utils.cpp

    objects/objects3d.h objects/truck.cpp
)
target_link_libraries( AR ${OpenCV_LIBS} )