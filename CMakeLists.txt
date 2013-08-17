# To create 64bit projects, add "Win64" to end of generator, like so:
# cmake CMakeLists.txt -G "Visual Studio 11 Win64"

# We want at least some arbitrary cmake version for this and a project name
cmake_minimum_required (VERSION 2.6)
project (MayaShapeSample)


# Set the name of the plugin
SET(MAYA_PLUGIN_NAME "MayaShapeSample")
# Set the path to the Maya installation whose libraries and headers you want to compile against.
SET(MAYA_DIRECTORY "C:/Program Files/Autodesk/Maya2013")
SET(MAYA_HEADERS_DIR "${MAYA_DIRECTORY}/include")
SET(MAYA_LIBRARY_DIR "${MAYA_DIRECTORY}/lib")

# Add the source files here
SET(SOURCE_FILES
  pluginMain.cpp
  pluginMain.h
)

source_group("" FILES ${SOURCE_FILES})

INCLUDE_DIRECTORIES(
  ${MAYA_HEADERS_DIR}
  ${MAYA_LIBRARY_DIR}
  ${Boost_INCLUDE_DIR}
)
LINK_DIRECTORIES(
  ${MAYA_LIBRARY_DIR}
  ${MAYA_HEADERS_DIR}
)

# When we use libraries from Maya they should be added here
SET(LIBRARIES
  Foundation.lib
  OpenMaya.lib
  OpenMayaAnim.lib
  OpenMayaUI.lib
)

# Definitions that is needed in order to compile the Maya headers.
SET(MAYA_DEFINITIONS _AFXDLL _MBCS NT_PLUGIN REQUIRE_IOSTREAM _BOOL)

ADD_LIBRARY(${MAYA_PLUGIN_NAME} SHARED ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${MAYA_PLUGIN_NAME} ${LIBRARIES})

SET_TARGET_PROPERTIES(${MAYA_PLUGIN_NAME} PROPERTIES COMPILE_DEFINITIONS "${MAYA_DEFINITIONS}")
SET_TARGET_PROPERTIES(${MAYA_PLUGIN_NAME} PROPERTIES OUTPUT_NAME "${MAYA_PLUGIN_NAME}")

# The suffix might need to be changed if on Linux, note sure yet.
SET_TARGET_PROPERTIES(${MAYA_PLUGIN_NAME} PROPERTIES SUFFIX ".mll")
SET_TARGET_PROPERTIES(${MAYA_PLUGIN_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(${MAYA_PLUGIN_NAME} PROPERTIES LINK_FLAGS "/export:initializePlugin /export:uninitializePlugin")