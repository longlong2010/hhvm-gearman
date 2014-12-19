FIND_PATH(GEARMAN_INCLUDE_DIR NAMES gearman.h 
	PATHS /usr/include/libgearman /usr/local/include/libgearman)

FIND_LIBRARY(GEARMAN_LIBRARY NAMES gearman PATHS /usr/lib /usr/local/lib)

IF (GEARMAN_INCLUDE_DIR)
	MESSAGE(STATUS "gearman Include dir: ${GEARMAN_INCLUDE_DIR}")
	MESSAGE(STATUS "libgearman library: ${GEARMAN_LIBRARY}")
ELSE()
	MESSAGE(FATAL_ERROR "Cannot find libgearman library")
ENDIF()

include_directories(${GEARMAN_INCLUDE_DIR})


HHVM_EXTENSION(gearman gearman.cpp)
HHVM_SYSTEMLIB(gearman ext_gearman.php)

target_link_libraries(gearman ${GEARMAN_LIBRARY})
