cmake_minimum_required (VERSION 2.8) 

PROJECT(testSURF)

# Requires OpenCV v2.4.9 or later
FIND_PACKAGE( OpenCV REQUIRED )
IF (${OpenCV_VERSION} VERSION_LESS 2.4.9)
    MESSAGE(FATAL_ERROR "OpenCV version is not compatible : ${OpenCV_VERSION}. testSURF requires atleast OpenCV v2.4.9")
ENDIF()

SET(SRC
    main.cpp
    SurfDetect.cpp
)

ADD_EXECUTABLE( ${PROJECT_NAME} ${SRC} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME}  ${OpenCV_LIBS} )
