# Install script for directory: /mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server/cmake_install.cmake")
  include("/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server_c/cmake_install.cmake")
  include("/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server_msg/cmake_install.cmake")
  include("/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server_oop/cmake_install.cmake")
  include("/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/publisher_subscriber/cmake_install.cmake")
  include("/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/publisher_subscriber_c/cmake_install.cmake")
  include("/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/publisher_subscriber_msg/cmake_install.cmake")
  include("/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/publisher_subscriber_multi/cmake_install.cmake")
  include("/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/publisher_subscriber_oop/cmake_install.cmake")
  include("/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/remapping/cmake_install.cmake")
  include("/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/cmdline_args/cmake_install.cmake")

endif()

