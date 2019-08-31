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
# API
amber_add_sources(common "api.hpp" FILTER "API")

# Memory
amber_add_sources(common "memory.hpp" FILTER "Memory/Memory")
amber_add_sources(common "memorymapping.hpp" FILTER "Memory/Memory Mapping")
amber_add_sources(common "mmu.hpp" FILTER "Memory/MMU")
amber_add_sources(common "ram.hpp" FILTER "Memory/RAM")
amber_add_sources(common "rom.hpp" FILTER "Memory/ROM")

# Debugging
amber_add_sources(common "debugger.hpp" "debugger.cpp" FILTER "Debugging/Debugger")
amber_add_sources(common "videoviewer.hpp" "videoviewer.cpp" FILTER "Debugging/Video Viewer")