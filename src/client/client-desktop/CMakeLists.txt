# Add executable
demu_add_executable(client-desktop)
demu_target_filter(client-desktop Client)

# Add dependencies
target_link_libraries(client-desktop PUBLIC client)

# Add sources
# Main
demu_add_sources(client-desktop "main.cpp" FILTER "Main")