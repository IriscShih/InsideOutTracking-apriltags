#-----------------------------------------------------------------------------
#
# OpenIGTLinkConfig.cmake - OpenIGTLink CMake configuration file for external projects.
#
# This file is configured by OpenIGTLink and used by the UseOpenIGTLink.cmake module
# to load OpenIGTLink's settings for an external project.

# Compute the installation prefix from this OpenIGTLinkConfig.cmake file location.
GET_FILENAME_COMPONENT(OpenIGTLink_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
GET_FILENAME_COMPONENT(OpenIGTLink_INSTALL_PREFIX "${OpenIGTLink_INSTALL_PREFIX}" PATH)
GET_FILENAME_COMPONENT(OpenIGTLink_INSTALL_PREFIX "${OpenIGTLink_INSTALL_PREFIX}" PATH)

# The OpenIGTLink include file directories.
SET(OpenIGTLink_INCLUDE_DIRS "${OpenIGTLink_INSTALL_PREFIX}/include/igtl")

# The OpenIGTLink library directories.
SET(OpenIGTLink_LIBRARY_DIRS "${OpenIGTLink_INSTALL_PREFIX}/lib/igtl")

# The C and C++ flags added by OpenIGTLink to the cmake-configured flags.
SET(OpenIGTLink_REQUIRED_C_FLAGS "")
SET(OpenIGTLink_REQUIRED_CXX_FLAGS "")
SET(OpenIGTLink_REQUIRED_LINK_FLAGS "")

# The OpenIGTLink Library version number
SET(OpenIGTLink_VERSION_MAJOR "1")
SET(OpenIGTLink_VERSION_MINOR "11")
SET(OpenIGTLink_VERSION_PATCH "0")

# The OpenIGTLink Protocol version number
SET(OpenIGTLink_PROTOCOL_VERSION "2")

# The location of the UseOpenIGTLink.cmake file.
SET(OpenIGTLink_USE_FILE "${OpenIGTLink_INSTALL_PREFIX}/lib/igtl/UseOpenIGTLink.cmake")

# The build settings file.
SET(OpenIGTLink_BUILD_SETTINGS_FILE "${OpenIGTLink_INSTALL_PREFIX}/lib/igtl/OpenIGTLinkBuildSettings.cmake")

# The library dependencies file.
SET(OpenIGTLink_LIBRARY_DEPENDS_FILE "${OpenIGTLink_INSTALL_PREFIX}/lib/igtl/OpenIGTLinkLibraryDepends.cmake")

# Whether OpenIGTLink was built with shared libraries.
SET(OpenIGTLink_BUILD_SHARED "OFF")

# Whether OpenIGTLink was built with Tcl wrapping support.
SET(OpenIGTLink_CSWIG_TCL "")
SET(OpenIGTLink_CSWIG_PYTHON "")
SET(OpenIGTLink_CSWIG_JAVA "")

# Path to CableSwig configuration used by OpenIGTLink.
SET(OpenIGTLink_CableSwig_DIR "")

# A list of all libraries for OpenIGTLink.  Those listed here should
# automatically pull in their dependencies.
#SET(OpenIGTLink_LIBRARIES OpenIGTLinkAlgorithms OpenIGTLinkStatistics OpenIGTLinkFEM)
SET(OpenIGTLink_LIBRARIES OpenIGTLink)

# The OpenIGTLink library dependencies.
IF(NOT OpenIGTLink_NO_LIBRARY_DEPENDS AND
    EXISTS "${OpenIGTLink_LIBRARY_DEPENDS_FILE}")
  INCLUDE("${OpenIGTLink_LIBRARY_DEPENDS_FILE}")
ENDIF(NOT OpenIGTLink_NO_LIBRARY_DEPENDS AND
  EXISTS "${OpenIGTLink_LIBRARY_DEPENDS_FILE}")
