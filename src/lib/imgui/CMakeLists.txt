# Add library
demu_add_library(imgui)
demu_target_filter(imgui Libraries)

# Add dependencies
find_package(OpenGL)

target_include_directories(imgui PUBLIC  
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>"
	"$<INSTALL_INTERFACE:include>"
)

target_link_libraries(imgui PUBLIC OpenGL::GL CONAN_PKG::sdl2)

# Add sources
demu_add_sources(imgui "imgui.natvis" FILTER "Natvis")
demu_add_sources(imgui "imconfig.h" "imgui_impl_opengl2.h" "imgui_impl_sdl.h" "imgui_memory_editor.h" "imgui.h" "imstb_rectpack.h" "imstb_textedit.h" "imstb_truetype.h" FILTER "Headers")
demu_add_sources(imgui "imgui_demo.cpp" "imgui_draw.cpp" "imgui_impl_opengl2.cpp" "imgui_impl_sdl.cpp" "imgui_widgets.cpp" "imgui.cpp" FILTER "Source")