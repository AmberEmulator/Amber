# Add library
demu_add_library(client)
demu_target_filter(client Client)

# Add dependencies
target_link_libraries(client PUBLIC common imgui)
target_include_directories(client PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>"
	"$<INSTALL_INTERFACE:include>"
)

# Add sources
# API
demu_add_sources(client "api.hpp" FILTER "API")

# Application
demu_add_sources(client "application.hpp" "application.cpp" FILTER "Client")