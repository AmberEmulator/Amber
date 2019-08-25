#include <client/disassembly.hpp>

#include <imgui/imgui.h>

using namespace Demu;
using namespace Client;

void Demu::Client::ShowDisassembly(const char* a_Name, DisassemblyState& a_State)
{
	auto& debugger = *(a_State.m_Debugger);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::BeginChild(a_Name, ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

	const auto& style = ImGui::GetStyle();
	const float font_height = ImGui::GetFontSize();
	const float row_height = font_height + style.FramePadding.y * 2.0f;
	const float font_y_offset = style.FramePadding.y;

	const size_t maximum_address_length = debugger.GetMaximumAddressWidth();
	const std::string test_string(maximum_address_length, 'A');
	const float address_width = ImGui::CalcTextSize(test_string.c_str()).x;
	const float space_width = ImGui::CalcTextSize(" ").x;

	const float address_offset = row_height + style.ItemInnerSpacing.x;
	const float instruction_offset = address_offset + address_width + style.ItemInnerSpacing.x + space_width * 2.0f;

	// Colors
	const ImU32 background_color = ImColor(style.Colors[ImGuiCol_TitleBg]);
	const ImU32 odd_row_color = ImColor(style.Colors[ImGuiCol_MenuBarBg]);
	const ImU32 font_color = ImColor(style.Colors[ImGuiCol_Text]);
	const ImU32 breakpoint_color = IM_COL32(255, 0, 0, 255);
	const ImU32 breakpoint_bar_color = ImColor(style.Colors[ImGuiCol_Border]);

	const ImVec2 window_top_left = ImGui::GetCursorScreenPos();
	const ImVec2 window_bottom_right = ImVec2(window_top_left.x + ImGui::GetWindowWidth(), window_top_left.y + ImGui::GetWindowHeight());

	auto& draw_list = *(ImGui::GetWindowDrawList());
	draw_list.AddRectFilled(window_top_left, window_bottom_right, background_color);
	draw_list.AddRectFilled(window_top_left, ImVec2(window_top_left.x + row_height, window_bottom_right.y), breakpoint_bar_color);

	uint64_t address = a_State.m_ViewAddress;
	for (size_t i = 0; i < 100; ++i)
	{
		ImGui::PushID(static_cast<int>(i));

		while (!debugger.IsValidAddress(address))
		{
			++address;
		}

		try
		{
			// Get instruction info
			const std::string address_name = debugger.GetAddressName(address);
			const std::string instruction_name = debugger.GetInstructionName(address);

			const ImVec2 row_top_left(window_top_left.x, window_top_left.y + i * row_height);
			const ImVec2 row_bottom_right(window_bottom_right.x, row_top_left.y + row_height);
			const float text_y = row_top_left.y + font_y_offset;

			// Background
			if (i % 2 == 0)
			{
				draw_list.AddRectFilled(ImVec2(row_top_left.x + row_height, row_top_left.y), row_bottom_right, odd_row_color);
			}

			// Breakpoint button
			ImGui::SetCursorScreenPos(row_top_left);
			if (ImGui::InvisibleButton("##Breakpoint", ImVec2(row_height, row_height)))
			{
				debugger.SetBreakpoint(address, !debugger.HasBreakpoint(address));
			}

			if (debugger.HasBreakpoint(address))
			{
				const ImVec2 circle_pos = ImVec2(row_top_left.x + row_height / 2.0f + 0.5f, row_top_left.y + row_height / 2.0f + 0.5f);

				draw_list.AddCircleFilled(circle_pos, row_height / 2.0f - 3.0f, breakpoint_color);
			}

			// Text
			draw_list.AddText(ImVec2(row_top_left.x + address_offset, text_y), font_color, address_name.c_str());
			draw_list.AddText(ImVec2(row_top_left.x + instruction_offset, text_y), font_color, instruction_name.c_str());

			// Go to next address
			address += debugger.GetInstructionSize(address);
		}
		catch (...)
		{
			ImGui::PopID();
			break;
		}

		ImGui::PopID();
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();
}