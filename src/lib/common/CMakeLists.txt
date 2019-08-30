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
amber_add_sources(common "addressmask.h" FILTER "Memory/Address Mask")
amber_add_sources(common "memory.h" FILTER "Memory/Memory")
amber_add_sources(common "memorymapping.hpp" FILTER "Memory/Memory Mapping")
amber_add_sources(common "mmu.hpp" FILTER "Memory/MMU")
amber_add_sources(common "ram.h" FILTER "Memory/RAM")
amber_add_sources(common "rom.h" FILTER "Memory/ROM")

# Debugging
amber_add_sources(common "debugger.hpp" "debugger.cpp" FILTER "Debugging/Debugger")