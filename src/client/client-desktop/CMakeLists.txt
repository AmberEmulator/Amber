# Add executable
amber_add_executable(client-desktop)
amber_target_filter(client-desktop Client)

# Add dependencies
target_link_libraries(client-desktop PUBLIC client)

# Add sources
# Main
amber_add_sources(client-desktop "main.cpp" FILTER "Main")