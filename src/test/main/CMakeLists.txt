# Add library
amber_add_library(test_main STATIC)

# Add dependencies
target_link_libraries(test_main CONAN_PKG::Catch2)
target_include_directories(test_main PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# Set filter
amber_target_filter(test_main "Tests")

# Add source files
# Main
amber_add_sources(test_main "main.cpp")