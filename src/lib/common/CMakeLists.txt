# Add library
demu_add_library(common)
demu_target_filter(common Libraries)

# Add dependencies
target_include_directories(common PUBLIC  
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>"
	"$<INSTALL_INTERFACE:include>"
)

# Add sources
# API
demu_add_sources(common "api.hpp" FILTER "API")

# Memory
demu_add_sources(common "addressmask.h" FILTER "Memory/Address Mask")
demu_add_sources(common "memory.h" FILTER "Memory/Memory")
demu_add_sources(common "mmu.h" FILTER "Memory/MMU")
demu_add_sources(common "ram.h" FILTER "Memory/RAM")
demu_add_sources(common "rom.h" FILTER "Memory/ROM")



demu_add_sources(common "foo.cpp")


# Debugging
demu_add_sources(common "disassembly.hpp" "disassembly.cpp" FILTER "Debugging/Disassembly")