#------------------------------------------------------------
SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_ADDITIONAL_VERSIONS "1.42" "1.42.0" "1.43" "1.43.0" "1.44" "1.44.0" "1.45" "1.45.0" "1.46" "1.46.0" "1.46.1")

#------------------------------------------------------------
SET(
  CMAKE_MODULE_PATH 
  ${PROJECT_SOURCE_DIR}/cmake  
  ${CMAKE_MODULE_PATH}
  )

FIND_PACKAGE(MAYA)
#------------------------------------------------------------
SET(MAYA_PLUGIN_NAME "MayaShapeSample")

INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/SAMPLE/include
  ${MAYA_INCLUDE_DIRS}
)

FILE(
  GLOB
  MAYA_PLUGIN_SOURCES
  ${PROJECT_SOURCE_DIR}/SAMPLE/src/*.cpp 
  )

FILE(
  GLOB
  MAYA_PLUGIN_HEADERS
  ${PROJECT_SOURCE_DIR}/SAMPLE/include/*.h 
  )

ADD_LIBRARY(
  ${MAYA_PLUGIN_NAME}
  SHARED
  ${MAYA_PLUGIN_SOURCES}
  ${MAYA_PLUGIN_HEADERS}
  )

TARGET_LINK_LIBRARIES(
  ${MAYA_PLUGIN_NAME}
  ${MAYA_Foundation_LIBRARY}
  ${MAYA_OpenMaya_LIBRARY}
  ${MAYA_MayaAnim_LIBRARY}
  ${MAYA_MayaUI_LIBRARY}
  )

#--------------------------------------------------------

# Definitions that is needed in order to compile the Maya headers.
IF(WIN32)
  SET(
    MAYA_DEFINITIONS
    _AFXDLL
    _MBCS
    NT_PLUGIN
    REQUIRE_IOSTREAM
    _BOOL
    )
ELSEIF(APPLE)
  SET(
    MAYA_DEFINITIONS
    MAC_PLUGIN
    OSMac_MachO_  
    UNIX 
    _BOOL 
    OSMac_
    REQUIRE_IOSTREAM
    )
ENDIF()

SET_TARGET_PROPERTIES(
  ${MAYA_PLUGIN_NAME}
  PROPERTIES
  COMPILE_DEFINITIONS
  "${MAYA_DEFINITIONS}"
  )

SET_TARGET_PROPERTIES(
  ${MAYA_PLUGIN_NAME}
  PROPERTIES
  OUTPUT_NAME
  "${MAYA_PLUGIN_NAME}"
  )

# The suffix might need to be changed if on Linux, note sure yet.
SET_TARGET_PROPERTIES(${MAYA_PLUGIN_NAME} PROPERTIES SUFFIX ".mll")
SET_TARGET_PROPERTIES(${MAYA_PLUGIN_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)

IF(WIN32)

  SET_TARGET_PROPERTIES(
    ${MAYA_PLUGIN_NAME}
    PROPERTIES
    LINK_FLAGS
    "/export:initializePlugin /export:uninitializePlugin"
    )

ENDIF()

#------------------------------------------------------------------
SET_TARGET_PROPERTIES(${MAYA_PLUGIN_NAME} PROPERTIES DEBUG_POSTFIX "D" )

#------------------------------------------------------------------
SET_TARGET_PROPERTIES(${MAYA_PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_SOURCE_DIR}/lib")
SET_TARGET_PROPERTIES(${MAYA_PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_SOURCE_DIR}/lib")
SET_TARGET_PROPERTIES(${MAYA_PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_SOURCE_DIR}/lib")
