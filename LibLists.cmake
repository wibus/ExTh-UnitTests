# Qt
FIND_PACKAGE(Qt5OpenGL REQUIRED)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)


# ExTh
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${UNITTESTS_SRC_DIR}/../ExperimentalTheatre/")
FIND_PACKAGE(ExperimentalTheatre REQUIRED)


# Global
SET(UNITTESTS_LIBRARIES
    ${ExTh_LIBRARIES})
SET(UNITTESTS_INCLUDE_DIRS
    ${UNITTESTS_SRC_DIR}
    ${ExTh_INCLUDE_DIRS})
SET(UNITTESTS_QT_MODULES
    OpenGL)