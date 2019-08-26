# Add library
amber_add_library(client)
amber_target_filter(client Client)

# Add dependencies
target_link_libraries(client PUBLIC common gameboy imgui)
target_include_directories(client PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>"
	"$<INSTALL_INTERFACE:include>"
)

# Add sources
# API
amber_add_sources(client "api.hpp" FILTER "API")

# Application
amber_add_sources(client "application.hpp" "application.cpp" FILTER "Client")

# Widgets
amber_add_sources(client "disassembly.hpp" "disassembly.cpp" FILTER "Widgets/Disassembly")
amber_add_sources(client "gameboywidgets.hpp" "gameboywidgets.cpp" FILTER "Widgets/Gameboy")