include(CMakeParseArguments)

function(demu_add_any _TARGET)
	# Compiler specific settings
	if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		# Enable multithreaded compilation
		target_compile_options("${_TARGET}" PUBLIC "/MP")

		# Increase warning level to 4
		target_compile_options("${_TARGET}" PRIVATE "/W4")

		# Disable the irrelevant warnings
		target_compile_options("${_TARGET}" PRIVATE "/wd4100") # Unreferenced function parameters (unused arguments)
		target_compile_options("${_TARGET}" PRIVATE "/wd4505") # Unreferenced local function has been removed (unused functions)

		# Enable edit and continue in debug mode
		target_compile_options("${_TARGET}" PRIVATE "$<$<CONFIG:Debug>:/ZI>")

		# Generate debug information in release mode
		target_compile_options("${_TARGET}" PRIVATE "$<$<CONFIG:Release>:/Zi>")

		# Generate full debug info in all modes
		set_property(TARGET "${_TARGET}" APPEND_STRING PROPERTY LINK_FLAGS " /DEBUG:FULL")
	endif()

	# Manually set language to C++
	set_target_properties("${_TARGET}" PROPERTIES LINKER_LANGUAGE CXX)
	target_compile_features("${_TARGET}" PUBLIC cxx_std_17)

	# Unfiltered files
	source_group(" " REGULAR_EXPRESSION ".*")
	
	# CMake Rules
	source_group("Code Generation\\CMake Rules" REGULAR_EXPRESSION ".rule")
endfunction()

function(demu_add_library _TARGET)
	# Create library
	add_library("${_TARGET}" ${ARGN} "")
	
	# Apply common settings to target
	demu_add_any("${_TARGET}")
endfunction()

function(demu_add_executable _TARGET)
	# Create executable
	add_executable("${_TARGET}" ${ARGN} "")
	
	# Apply common settings to target
	demu_add_any("${_TARGET}")
endfunction()

function(demu_add_test _TARGET)
	# Create exectutable
	demu_add_executable("${_TARGET}" ${ARGN})
	
	# Mark it as a test
	add_test(NAME "${_TARGET}" COMMAND "${_TARGET}")
	
	# Add it to the test folder
	set_target_properties("${_TARGET}" PROPERTIES FOLDER "Tests")
endfunction()

function(demu_target_filter _TARGET _FILTER)
	set_target_properties("${_TARGET}" PROPERTIES FOLDER "${_FILTER}")
endfunction()

function(demu_add_sources _TARGET)
	# Parse arguments
	set(_OPTIONS)
	set(_ONE_VALUE_ARGS FILTER)
	set(_MULTI_VALUE_ARGS RESOURCES)
	cmake_parse_arguments(_SOURCES "${_OPTIONS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN})
	
	set(_SOURCES ${_SOURCES_UNPARSED_ARGUMENTS})
	
	# Add sources to target
	foreach(_SOURCE ${_SOURCES})
		target_sources("${_TARGET}" PRIVATE "${_SOURCE}")
	endforeach()

	# Apply filter
	if(DEFINED _SOURCES_FILTER)
		string(REPLACE "/" "\\" _SOURCES_FILTER "${_SOURCES_FILTER}")
		source_group("${_SOURCES_FILTER}" FILES ${_SOURCES})
	endif()
endfunction()