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
amber_add_sources(gameboy "instructionbuilder.hpp" FILTER "CPU/Instruction Builder")
amber_add_sources(gameboy "instructionset.hpp" FILTER "CPU/Instruction Set")
amber_add_sources(gameboy "opcode.hpp" "extendedopcode.hpp" FILTER "CPU/Opcodes")
amber_add_sources(gameboy "register.hpp" "register.cpp" FILTER "CPU/Register")

# PPU
amber_add_sources(gameboy "ppu.hpp" "ppu.cpp" FILTER "PPU/PPU")

# Memory
amber_add_sources(gameboy "mmu.hpp" "mmu.cpp" FILTER "Memory/MMU")

# Input
amber_add_sources(gameboy "joypad.hpp" "joypad.cpp" FILTER "Input/Joypad")
amber_add_sources(gameboy "joypadbutton.hpp" FILTER "Input/Joypad Button")

# Device
amber_add_sources(gameboy "device.hpp" "device.cpp" FILTER "Device/Device")
amber_add_sources(gameboy "devicedescription.hpp" "devicedescription.cpp" FILTER "Device/Device Description")

# Debugging
amber_add_sources(gameboy "debugger.hpp" "debugger.cpp" FILTER "Debugging/Debugger")
amber_add_sources(gameboy "videoviewer.hpp" "videoviewer.cpp" FILTER "Debugging/Video Viewer")