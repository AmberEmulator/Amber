# Add library
amber_add_library(gameboy)
amber_target_filter(gameboy Libraries)

# Add dependencies
target_link_libraries(gameboy PUBLIC common)
target_include_directories(gameboy PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>"
	"$<INSTALL_INTERFACE:include>"
)

# Add sources
# Natvis
amber_add_sources(gameboy "gameboy.natvis" FILTER "Natvis")

# API
amber_add_sources(gameboy "api.hpp" FILTER "API")

# Gameboy
amber_add_sources(gameboy "gameboytype.hpp" FILTER "Gameboy")

# CPU
amber_add_sources(gameboy "cpu.hpp" "cpu.cpp" FILTER "CPU/CPU")
amber_add_sources(gameboy "registers.hpp" FILTER "CPU/Registers")
amber_add_sources(gameboy "instruction.hpp" "extendedinstruction.hpp" FILTER "CPU/Instructions")

# Debugging
amber_add_sources(gameboy "debugger.hpp" "debugger.cpp" FILTER "Debugging/Debugger")
amber_add_sources(gameboy "videoviewer.hpp" "videoviewer.cpp" FILTER "Debugging/Video Viewer")