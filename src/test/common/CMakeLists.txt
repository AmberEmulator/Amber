# Add Test
demu_add_test(test_common)

# Add dependencies
target_link_libraries(test_common test_main)

# Add source files
demu_add_sources(test_common "addressmask.cpp" FILTER "Memory/Address Mask")