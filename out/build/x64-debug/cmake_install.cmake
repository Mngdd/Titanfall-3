<<<<<<< Updated upstream
# Install script for directory: C:/Documents/GitHub/Titanfall-3/CMAKE/Titanfall-3

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Documents/GitHub/Titanfall-3/CMAKE/Titanfall-3/out/install/x64-debug")
=======
# Install script for directory: C:/Documents/GitHub/Titanfall-3

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Documents/GitHub/Titanfall-3/out/install/x64-debug")
>>>>>>> Stashed changes
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
<<<<<<< Updated upstream
  # Include the install script for each subdirectory.
  include("C:/Documents/GitHub/Titanfall-3/CMAKE/Titanfall-3/out/build/x64-debug/Titanfall-3/cmake_install.cmake")

=======
  # Include the install script for the subdirectory.
  include("C:/Documents/GitHub/Titanfall-3/out/build/x64-debug/Titanfall-3/cmake_install.cmake")
>>>>>>> Stashed changes
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
<<<<<<< Updated upstream
file(WRITE "C:/Documents/GitHub/Titanfall-3/CMAKE/Titanfall-3/out/build/x64-debug/${CMAKE_INSTALL_MANIFEST}"
=======
file(WRITE "C:/Documents/GitHub/Titanfall-3/out/build/x64-debug/${CMAKE_INSTALL_MANIFEST}"
>>>>>>> Stashed changes
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")