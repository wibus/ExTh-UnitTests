## Headers ##

# All the header files #
SET(UNITTESTS_HEADERS
    ${UNITTESTS_SRC_DIR}/catch.hpp)



## Sources ##

# All the source files #
SET(UNITTESTS_SOURCES
    ${UNITTESTS_SRC_DIR}/main.cpp
    ${UNITTESTS_SRC_DIR}/PropRoom3D.cpp)



## Global ##
SET(UNITTESTS_CONFIG_FILES
    ${UNITTESTS_SRC_DIR}/CMakeLists.txt
    ${UNITTESTS_SRC_DIR}/FileLists.cmake
    ${UNITTESTS_SRC_DIR}/LibLists.cmake)
	
SET(UNITTESTS_SRC_FILES
    ${UNITTESTS_HEADERS}
    ${UNITTESTS_SOURCES}
    ${UNITTESTS_CONFIG_FILES}
    ${UNITTESTS_MOC_CPP_FILES})
