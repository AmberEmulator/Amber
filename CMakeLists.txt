cmake_minimum_required(VERSION 3.8)

# Create project
project(demu CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set install prefix
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE TYPE PATH FORCE)
endif()

# Install conan packages
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
	message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
	file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.13/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
include("${CMAKE_BINARY_DIR}/conan.cmake")

conan_cmake_run(
	CONANFILE "conanfile.txt"
	BASIC_SETUP CMAKE_TARGETS
	BUILD missing)

# Add cmake module paths
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Include function scripts
include(targets)

# Add subdirectories
add_subdirectory(src)