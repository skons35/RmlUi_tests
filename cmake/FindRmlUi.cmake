#inspired from original findPackage coming from BitRiot github repo

#IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#	message(STATUS "build type : " ${CMAKE_BUILD_TYPE})
#	message(STATUS "Tuning Debug library names...")	
#ELSE()
#ENDIF()

FIND_PATH(RMLUI_INCLUDE RmlUi/Core.h  # find RmlUi/Core.h and set it in RMLUI_INCLUDE
  HINTS
  PATH_SUFFIXES Include  # postfix the path 'Include' when searching the paths below for 'RmlUi/Core.h'
  PATHS
  #$ENV{RMLUI}  # Use environment variable RMLUI and see if $RMLUI/include/RmlUi/Core.h exists
  /usr/local
)

# VA : compose the debug and release libraries options 
#      (assuming debug has 'd' postfix added to name)

# 1) release version found ?:
FIND_LIBRARY(RMLUI_CORE RmlCore
  HINTS
  #PATH_SUFFIXES lib lib64 libs/Win32 libs/Win64
  PATH_SUFFIXES lib build  # VA fix
  PATHS
  #$ENV{RMLUI} # Use environment variable RMLUI and see if $RMLUI/build contains  RmlCore lib
  /usr/local
)

FIND_LIBRARY(RMLUI_DEBUGGER RmlDebugger
  HINTS
  #PATH_SUFFIXES lib lib64
  PATH_SUFFIXES lib build  # VA fix
  PATHS
  #$ENV{RMLUI} # Use environment variable RMLUI and see if $RMLUI/build contains  RmlDebugger lib
  /usr/local
)

# 1) debug version found ?:
FIND_LIBRARY(RMLUI_CORE_DEBUG RmlCored
  HINTS
  #PATH_SUFFIXES lib lib64 libs/Win32 libs/Win64
  PATH_SUFFIXES lib build  # VA fix
  PATHS
  #$ENV{RMLUI} # Use environment variable RMLUI and see if $RMLUI/build contains  RmlCore lib
  /usr/local
)

FIND_LIBRARY(RMLUI_DEBUGGER_DEBUG RmlDebuggerd
  HINTS
  #PATH_SUFFIXES lib lib64
  PATH_SUFFIXES lib build  # VA fix
  PATHS
  #$ENV{RMLUI} # Use environment variable RMLUI and see if $RMLUI/build contains  RmlDebugger lib
  /usr/local
)

  
SET(RMLUI_FOUND "NO")
IF(RMLUI_CORE AND RMLUI_DEBUGGER AND RMLUI_INCLUDE)
  SET(RMLUI_FOUND "YES")
  #SET(RMLUI_LIBRARIES ${RMLUI_CORE} ${RMLUI_DEBUGGER})
  #default linking (optimized): 
  SET(RMLUI_LIBRARIES "optimized" ${RMLUI_CORE} "optimized" ${RMLUI_DEBUGGER})
  message(STATUS "composed linking library options : " ${RMLUI_LIBRARIES})
  # add debug librarie if found :
  IF(RMLUI_CORE_DEBUG AND RMLUI_DEBUGGER_DEBUG)
	message(STATUS "adding found DEBUG libraries to linking options...")
	SET(RMLUI_LIBRARIES ${RMLUI_LIBRARIES} "debug" ${RMLUI_CORE_DEBUG} "debug" ${RMLUI_DEBUGGER_DEBUG})
	message(STATUS "composed linking library options : " ${RMLUI_LIBRARIES})
  ENDIF()
ENDIF()

#FIND_PACKAGE_HANDLE_STANDARD_ARGS(RmlUi
#	REQUIRED_VARS RMLUI_CORE RMLUI_INCLUDE)
