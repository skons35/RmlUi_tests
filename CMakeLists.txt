# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8) # allow use of FindFreetype, ...

project ("rmlui_tests")


# search for locally installed RmlUi lib :
set(RMLUI_STATIC true)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(RmlUi REQUIRED)

message(STATUS "RmlUi include dir.:"  ${RMLUI_INCLUDE}) 
message(STATUS "RmlUi libraries   :"  ${RMLUI_LIBRARIES}) 

include_directories(${RMLUI_INCLUDE})

# search for usual installed SDL2 libs :
find_package(SDL2 REQUIRED)
#find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

message(STATUS "SDL2 include dir.:"  ${SDL2_INCLUDE_DIR}) 
message(STATUS "SDL2, SDL2_TTF, SDL2_IMAGE libraries :"  ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY} ${SDL2IMAGE_LIBRARY} ) 

include_directories(${SDL2_INCLUDE_DIR})
#include_directories(${SDL2_MIXER_INCLUDE_DIR})
#include_directories(${SDL2_TTF_INCLUDE_DIR})
#include_directories(${SDL2IMAGE_INCLUDE_DIR})


find_package(Freetype REQUIRED)

message(STATUS "Freetype include dir.:"  ${FREETYPE_INCLUDE_DIRS}) 
message(STATUS "Freetype  library :" ${FREETYPE_LIBRARIES})

include_directories(${FREETYPE_INCLUDE_DIRS})

#find_package(OpenGL REQUIRED)


#Adding display of platform infos :
message(STATUS "Platform :"  ${CMAKE_HOST_SYSTEM_NAME})     # for instance : MorphOS, Linux, ...
#message(STATUS "System   :"  ${CMAKE_SYSTEM})              # for instance : MorphOS-3.13 , ...

# add Linux/Windows specific FreeGlut specific lookup
#if(NOT "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "MorphOS")
#	# add Windows & Linux lookup for Freeglut package :
#	FIND_PACKAGE(GLUT REQUIRED)  # FindGLUT.cmake is going to look for NAMES : glut glut32 freeglut
#	include_directories( ${GLUT_INCLUDE_DIR} )
#else(NOT "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "MorphOS")
#	# for Morphos, Tweaking gl glut related dependencies
#	target_compile_options(-noixemul)
#endif(NOT "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "MorphOS")

#set(EXTRA_LIBRARIES "")
## extra libs required for Linux
#if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")
#	set(EXTRA_LIBRARIES ${EXTRA_LIBRARIES} "GL" "GLU") #${DEP_LIBRARIES} GL GLU)
#endif("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")

#message(STATUS "GLUT_LIBRARIES : "  ${GLUT_LIBRARIES}) 
#message(STATUS "EXTRA_LIBRARIES : "  ${EXTRA_LIBRARIES})  

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/RmlUiBackendsLib/inc)
message(STATUS "INCLUDE_DIRECTORIES :"  ${CMAKE_SOURCE_DIR}/RmlUiBackendsLib/inc)  

FILE(GLOB sources ${CMAKE_SOURCE_DIR}/RmlUiBackendsLib/src/*.cpp)
ADD_LIBRARY(rmluiBackendsLib ${sources})

# main prog :
ADD_EXECUTABLE(rmluiTester ${CMAKE_SOURCE_DIR}/main.cpp)

target_link_libraries(rmluiTester rmluiBackendsLib ${RMLUI_LIBRARIES} ${FREETYPE_LIBRARIES} ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY} ${SDL2IMAGE_LIBRARY})

if(NOT "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "MorphOS")
    target_link_libraries(rmluiTester rmluiBackendsLib ${RMLUI_LIBRARIES} ${FREETYPE_LIBRARIES} ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY} ${SDL2IMAGE_LIBRARY})
else(NOT "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "MorphOS")
    message(STATUS "Morphos specific tweaking : libs c and m linking, and add link flag -noixemul")
	target_link_libraries(rmluiTester rmluiBackendsLib ${RMLUI_LIBRARIES} ${FREETYPE_LIBRARIES} ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY} ${SDL2IMAGE_LIBRARY} GL c m)
	SET_TARGET_PROPERTIES(rmluiTester PROPERTIES LINK_FLAGS -noixemul)
    # target_link_libraries(sceneTester sceneTestsLib m Gl glut)
endif(NOT "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "MorphOS")


# install part related :

SET_PROPERTY(TARGET rmluiTester
  PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  )
 
INSTALL(TARGETS rmluiBackendsLib DESTINATION lib)
INSTALL(TARGETS rmluiTester DESTINATION bin)

# could also copy an EXISITNG 'run' folder into destination folder : ('run' must exist, or cmake error)
#INSTALL(DIRECTORY run DESTINATION ${CMAKE_BINARY_DIR}/run)
