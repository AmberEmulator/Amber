# Add Test
amber_add_test(test_common)

# Add dependencies
target_link_libraries(test_common test_main common)

# Add source files
# Memory
amber_add_sources(test_common "ram.cpp" FILTER "Memory/RAM")

# Recording
amber_add_sources(test_common "bytereader.cpp" FILTER "Recording/Byte Reader")
amber_add_sources(test_common "bytewriter.cpp" FILTER "Recording/Byte Writer")
amber_add_sources(test_common "recorder.cpp" FILTER "Recording/Recorder")