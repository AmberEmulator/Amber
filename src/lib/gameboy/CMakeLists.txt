# Add library
demu_add_library(gameboy)
demu_target_filter(gameboy Libraries)

# Add dependencies
target_link_libraries(gameboy PUBLIC common)
target_include_directories(gameboy PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>"
	"$<INSTALL_INTERFACE:include>"
)

# Add sources
# API
demu_add_sources(gameboy "api.hpp" FILTER "API")

# Gameboy
demu_add_sources(gameboy "gameboytype.hpp" FILTER "Gameboy")

# CPU
demu_add_sources(gameboy "cpu.hpp" "cpu.cpp" FILTER "CPU/CPU")
demu_add_sources(gameboy "registers.hpp" FILTER "CPU/Registers")
demu_add_sources(gameboy "instruction.hpp" "extendedinstruction.hpp" FILTER "CPU/Instructions")

# Debugging
demu_add_sources(gameboy "disassembly.hpp" "disassembly.cpp" FILTER "Debugging/Disassembly")