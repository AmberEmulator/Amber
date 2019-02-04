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
# Memory
demu_add_sources(common "memory.h" "memory.cpp" FILTER "Memory/Memory")
demu_add_sources(common "mmu.h" "mmu.cpp" FILTER "Memory/MMU")