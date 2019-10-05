# Add library
amber_add_library(common)
amber_target_filter(common Libraries)

# Add dependencies
target_include_directories(common PUBLIC  
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>"
	"$<INSTALL_INTERFACE:include>"
)

# Add sources
# Natvis
amber_add_sources(common "common.natvis" FILTER "Natvis")

# API
amber_add_sources(common "api.hpp" FILTER "API")

# Exceptions
amber_add_sources(common "exception.hpp" "exception.cpp" FILTER "Exception")

# Memory
amber_add_sources(common "memory.hpp" FILTER "Memory/Memory")
amber_add_sources(common "memorymapping.hpp" FILTER "Memory/Memory Mapping")
amber_add_sources(common "mmu.hpp" FILTER "Memory/MMU")
amber_add_sources(common "ram.hpp" FILTER "Memory/RAM")
amber_add_sources(common "rom.hpp" FILTER "Memory/ROM")

# CPU
amber_add_sources(common "cpuhelper.hpp" FILTER "CPU/CPU")
amber_add_sources(common "instructionbuilder.hpp" FILTER "CPU/Instruction Builder")
amber_add_sources(common "instructionset.hpp" FILTER "CPU/Instruction Set")
amber_add_sources(common "register.hpp" FILTER "CPU/Register")

# Recording
amber_add_sources(common "recordable.hpp" "recordable.cpp" FILTER "Recording/Recordable")
amber_add_sources(common "recorder.hpp" "recorder.cpp" FILTER "Recording/Recorder")
amber_add_sources(common "recorderchanneldescription.hpp" "recorderchanneldescription.cpp" FILTER "Recording/Recorder Channel Description")
amber_add_sources(common "recorderdescription.hpp" "recorderdescription.cpp" FILTER "Recording/Recorder Description")
amber_add_sources(common "recordermemberdescription.hpp" "recordermemberdescription.cpp" FILTER "Recording/Recorder Member Description")
amber_add_sources(common "recordermembertype.hpp" FILTER "Recording/Recorder Member Type")
amber_add_sources(common "bytereader.hpp" "bytereader.cpp" FILTER "Recording/Byte Reader")
amber_add_sources(common "bytewriter.hpp" "bytewriter.cpp" FILTER "Recording/Byte Writer")

# Debugging
amber_add_sources(common "breakpointdescription.hpp" "breakpointdescription.cpp" FILTER "Debugging/Breakpoint Description")
amber_add_sources(common "breakpointcondition.hpp" "breakpointcondition.cpp" FILTER "Debugging/Breakpoint Condition")
amber_add_sources(common "breakpointconditiontype.hpp" FILTER "Debugging/Breakpoint Condition Type")
amber_add_sources(common "debugger.hpp" "debugger.cpp" FILTER "Debugging/Debugger")
amber_add_sources(common "videoviewer.hpp" "videoviewer.cpp" FILTER "Debugging/Video Viewer")