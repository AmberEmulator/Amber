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
amber_add_sources(gameboy "opcode.hpp" "extendedopcode.hpp" FILTER "CPU/Opcodes")

# PPU
amber_add_sources(gameboy "lcdmode.hpp" FILTER "PPU/LCD Mode")
amber_add_sources(gameboy "pixel.hpp" "pixel.cpp" FILTER "PPU/Pixel")
amber_add_sources(gameboy "pixelfifo.hpp" "pixelfifo.cpp" FILTER "PPU/Pixel FIFO")
amber_add_sources(gameboy "pixelsource.hpp" FILTER "PPU/Pixel Source")
amber_add_sources(gameboy "ppu.hpp" "ppu.cpp" FILTER "PPU/PPU")
amber_add_sources(gameboy "ppuobserver.hpp" "ppuobserver.cpp" FILTER "PPU/PPU Observer")
amber_add_sources(gameboy "tilefetcher.hpp" "tilefetcher.cpp" FILTER "PPU/Tile Fetcher")

# MMU
amber_add_sources(gameboy "mmu.hpp" "mmu.cpp" FILTER "MMU/MMU")

# Cartridges
amber_add_sources(gameboy "cartridge.hpp" "cartridge.cpp" FILTER "Cartridge/Cartridge")
amber_add_sources(gameboy "cartridgeheader.hpp" "cartridgeheader.cpp" FILTER "Cartridge/Cartridge Header")
amber_add_sources(gameboy "cartridgeloader.hpp" "cartridgeloader.cpp" FILTER "Cartridge/Cartridge Loader")
amber_add_sources(gameboy "cartridgetype.hpp" FILTER "Cartridge/Cartridge Type")

amber_add_sources(gameboy "basiccartridge.hpp" "basiccartridge.cpp" FILTER "Cartridge/Cartridges/Basic Cartridge")
amber_add_sources(gameboy "mbc1cartridge.hpp" "mbc1cartridge.cpp" FILTER "Cartridge/Cartridges/MBC1 Cartridge")
amber_add_sources(gameboy "mbc2cartridge.hpp" "mbc2cartridge.cpp" FILTER "Cartridge/Cartridges/MBC2 Cartridge")

# Input
amber_add_sources(gameboy "joypad.hpp" "joypad.cpp" FILTER "Input/Joypad")
amber_add_sources(gameboy "joypadbutton.hpp" FILTER "Input/Joypad Button")

# Device
amber_add_sources(gameboy "device.hpp" "device.cpp" FILTER "Device/Device")
amber_add_sources(gameboy "devicedescription.hpp" "devicedescription.cpp" FILTER "Device/Device Description")

# Debugging
amber_add_sources(gameboy "debugger.hpp" "debugger.cpp" FILTER "Debugging/Debugger")
amber_add_sources(gameboy "event.hpp" FILTER "Debugging/Event")
amber_add_sources(gameboy "videoviewer.hpp" "videoviewer.cpp" FILTER "Debugging/Video Viewer")