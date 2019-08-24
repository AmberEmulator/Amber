# Add Test
demu_add_test(test_gameboy)

# Add dependencies
target_link_libraries(test_gameboy test_main gameboy)

# Add source files
# CPU
demu_add_sources(test_gameboy "instruction_add.cpp" FILTER "CPU/Instructions")