PROJECT (Code)

cmake_minimum_required (VERSION 2.6)

# make a lib directory in the build directory
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

# tell cmake that the library goes in the library directory
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)

# make a binary directory in the build directory
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# tell cmake that the binaries goes in the binary directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/home/radwann/Documents/MasterThesis/Code")
#set(JPEG_INCLUDE_DIR "/home/noha/Documents/UniversityofFreiburg/MasterThesis/jpeg-9a")
set(PNG_INCLUDE_DIR "/usr/include/libpng12")#"/home/noha/Documents/UniversityofFreiburg/MasterThesis/libpng-1.6.12")

#QT5 stuff
#set(QT_INCLUDE_PATH "/home/noha/Qt/5.3/gcc_64/lib")
#include_directories(${QT_INCLUDE_PATH})
#set(QTWidget_NAMES ${QTWidget_NAMES} libQt5Widgets.so)
#find_package(Qt5Widgets REQUIRED)
#find_package(Qt5Network REQUIRED)
#find_package(Qt5PrintSupport REQUIRED)
#find_package(Qt5Declarative REQUIRED)

#include_directories(${Qt5Widgets_INCLUDES})
#include_directories(${Qt5Network_INCLUDES})
#include_directories(${Qt5PrintSupport_INCLUDES})
#include_directories(${Qt5Declarative_INCLUDES})

#add_definitions(${Qt5Widgets_DEFINITIONS})
#add_definitions(${Qt5Network_DEFINITIONS})
#add_definitions(${Qt5PrintSupport_DEFINITIONS})
#add_definitions(${Qt5Declarative_DEFINITIONS})

#set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
#set(CMAKE_CXX_FLAGS "${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")
#set(CMAKE_CXX_FLAGS "${Qt5PrintSupport_EXECUTABLE_COMPILE_FLAGS}")
#set(CMAKE_CXX_FLAGS "${Qt5Declarative_EXECUTABLE_COMPILE_FLAGS}")

#GUI
#SET(Visualization_SOURCES visualization/mainwindow.cpp)
#SET(Visualization_HEADERS visualization/mainwindow.h)
#QT5_WRAP_CPP(Visualization_HEADERS_MOC ${Visualization_HEADERS})
#ADD_DEFINITIONS(-DVisualization_LIBRARY)
#ADD_LIBRARY(Visualization SHARED ${Visualization_SOURCES} ${Visualization_HEADERS_MOC})
#target_link_libraries(Visualization ${Qt5Widgets_LIBRARIES} ${Qt5Network_LIBRARIES} ${Qt5PrintSupport_LIBRARIES})# ${Qt5Declarative_LIBRARIES})

#find opencv
FIND_PACKAGE(OpenCV REQUIRED)

#find ccv 
find_package(CCV REQUIRED)
include_directories(${CCV_INCLUDE_DIR})
set(LIBS ${CCV_LIBRARY})

#find wordnet blast
set(WNB_INCLUDE_PATH "/home/radwann/Documents/Software/wnb-0.6/wnb/core")
include_directories(${WNB_INCLUDE_PATH})
set(WNB_NAMES ${WNB_NAMES} libwnb.a)
FIND_LIBRARY(WNB_LIBRARY NAMES ${WNB_NAMES}
	PATHS
	/usr/local/lib
	/home/radwann/Documents/Software/wnb-0.6/lib/
)

MESSAGE(STATUS "Found WNB: ${WNB_LIBRARY}")

set(LIBS ${LIBS} ${WNB_LIBRARY})

find_package(JPEG REQUIRED)
#include_directories(${JPEG_INCLUDE_DIR})
set(LIBS ${LIBS} ${JPEG_LIBRARY})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
message(STATUS "png dir: ${PNG_INCLUDE_DIR}") 
set(LIBS ${LIBS} ${PNG_LIBRARY})

# find tesseract
set(TESSERACT_INCLUDE_PATH "/usr/local/include/tesseract")
include_directories(${TESSERACT_INCLUDE_PATH})

SET(TESSERACT_NAMES ${TESSERACT_NAMES} libtesseract.so.3)
FIND_LIBRARY(TESSERACT_LIBRARY NAMES ${TESSERACT_NAMES}
  PATHS
  /usr/lib
  /home/radwann/Documents/Software/tesseract-3.03/api
)

MESSAGE(STATUS "Found Tesseract: ${TESSERACT_LIBRARY}")

set(LIBS ${LIBS} ${TESSERACT_LIBRARY})

#find leptonica
set(LEPTONICA_INCLUDE_PATH "/usr/local/include/leptonica")
include_directories(${LEPTONICA_INCLUDE_PATH})

SET(LEPTONICA_NAMES ${LEPTONICA_NAMES} liblept.so)
FIND_LIBRARY(LEPTONICA_LIBRARY NAMES ${LEPTONICA_NAMES}
	PATHS
	/usr/local/lib
	/home/radwann/Documents/Software/leptonica-1.70/
)

MESSAGE(STATUS "Found Leptonica: ${LEPTONICA_LIBRARY}")

set(LIBS ${LIBS} ${LEPTONICA_LIBRARY})

#find curl
FIND_PACKAGE(CURL)
IF(CURL_FOUND)
  INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
  SET(LIBS ${LIBS} ${CURL_LIBRARIES} )
ELSE(CURL_FOUND)
  MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
ENDIF(CURL_FOUND)

MESSAGE(STATUS "Found cUrl: ${CURL_LIBRARY}")

set(LIBS ${LIBS} ${CURL_LIBRARY})

#find jansson
set(JANSSON_INCLUDE_PATH "usr/local/include")
include_directories(${JANSSON_INCLUDE_PATH})

set(JANSSON_NAMES ${JANSSON_NAMES} libjansson.a)
FIND_LIBRARY(JANSSON_LIBRARY NAMES ${JANSSON_NAMES}
	PATHS
	/usr/local/lib
	/home/radwann/Documents/Software/jansson-2.7/
)

MESSAGE(STATUS "Found Jansson: ${JANSSON_LIBRARY}")

set(LIBS ${LIBS} ${JANSSON_LIBRARY})


#find eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})


message(STATUS "linked libs: ${LIBS}")

#add the CURLWrapper files
add_library(CurlWrapper SHARED src/CURLWrapper.cpp)
add_library(LM SHARED src/LM_algorithm.cpp include/LM_algorithm.hpp)
add_library(InitWorld SHARED src/init_world.cpp include/init_world.hpp)

#add_executable(CurlWrapper src/CURLWrapper.cpp)
add_executable(Main src/main.cpp)
#add_executable(Test src/test.cpp)
#add_executable(Evaluate src/evaluate_results.cpp)
#add_executable(WordMap src/wordMap.cpp)
#add_executable(LM src/LM_algorithm.cpp)
#add_executable(InitWorld src/init_world.cpp)
add_executable(VL src/VL_algorithm.cpp)
add_executable(MapParser src/map_parser.cpp)
#add_executable(NMAlg src/nm_algorithm.cpp)
#target_link_libraries(CurlWrapper ${LIBS})
target_link_libraries(Main ${LIBS} CurlWrapper)
#target_link_libraries(Main Visualization ${Qt5Widgets_LIBRARIES} ${Qt5Network_LIBRARIES} ${Qt5PrintSupport_LIBRARIES})# ${Qt5Declarative_LIBRARIES})
#target_link_libraries(Test ${LIBS} CurlWrapper)
#target_link_libraries(Evaluate ${LIBS})
target_link_libraries(Main ${OpenCV_LIBS})
#target_link_libraries(WordMap ${LIBS} CurlWrapper ${OpenCV_LIBS})
#target_link_libraries(LM ${LIBS})
#target_link_libraries(InitWorld ${LIBS})
target_link_libraries(VL ${LIBS} CurlWrapper ${OpenCV_LIBS} LM InitWorld)
target_link_libraries(MapParser ${LIBS})
#target_link_libraries(NMAlg ${LIBS})
# Link against the required libraries in OpenCV >=2.2
#IF(OPENCV2_FOUND)
#    SET( OPENCV2_INSTALLED 1)
#    message( STATUS "Using OpenCV >= 2.2" )
#	IF(WIN32)
#		IF(MINGW)
#			TARGET_LINK_LIBRARIES(Main ${OPENCV2_LIBS})
#		ELSE(MINGW)
#			FOREACH (LIB ${OPENCV2_DEBUG_LIBS})
#				TARGET_LINK_LIBRARIES(Main
#				debug ${LIB})
#			ENDFOREACH(LIB)
#			FOREACH (LIB ${OPENCV2_RELEASE_LIBS})
#				TARGET_LINK_LIBRARIES(Main
#				optimized ${LIB})
#			ENDFOREACH(LIB)
#		ENDIF(MINGW)
#	ELSE(WIN32)
#		TARGET_LINK_LIBRARIES(Main opencv_flann opencv_nonfree opencv_highgui opencv_core opencv_features2d opencv_imgproc)
#	ENDIF(WIN32)
#ENDIF(OPENCV2_FOUND)
