# Add Test
amber_add_test(test_common)

# Add dependencies
target_link_libraries(test_common test_main common)

# Add source files
amber_add_sources(test_common "addressmask.cpp" FILTER "Memory/Address Mask")
amber_add_sources(test_common "ram.cpp" FILTER "Memory/RAM")