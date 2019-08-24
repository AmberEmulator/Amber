#include <client/disassembly.hpp>

#include <imgui/imgui.h>

using namespace Demu;
using namespace Client;

void Demu::Client::ShowDisassembly(const char* a_Name, DisassemblyState& a_State)
{
	auto& disassembly = *(a_State.m_Disassembly);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::BeginChild(a_Name, ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

	const auto& style = ImGui::GetStyle();
	auto& draw_list = *(ImGui::GetWindowDrawList());
	const float font_height = ImGui::GetFontSize();
	const float row_height = font_height + style.FramePadding.y;
	const ImVec2 window_top_left = ImGui::GetCursorScreenPos();

	uint64_t address = a_State.m_ViewAddress;
	for (size_t i = 0; i < 100; ++i)
	{
		const ImVec2 row_begin(window_top_left.x, window_top_left.y + i * (row_height + 1.0f));
		ImGui::PushID(static_cast<int>(i));
		ImGui::SetCursorScreenPos(row_begin);

		while (!disassembly.IsValidAddress(address))
		{
			++address;
		}

		try
		{
			if (ImGui::Button("##Breakpoint", ImVec2(row_height, row_height)))
			{
				disassembly.SetBreakpoint(address, !disassembly.HasBreakpoint(address));
			}

			if (disassembly.HasBreakpoint(address))
			{
				const ImVec2 circle_pos = ImVec2(row_begin.x + row_height / 2.0f + 0.5f, row_begin.y + row_height / 2.0f + 0.5f);

				draw_list.AddCircleFilled(circle_pos, row_height / 2.0f - 1.0f, IM_COL32(255, 0, 0, 255));
			}


			bool selected = address == a_State.m_SelectedAddress;
			ImGui::SameLine();
			ImGui::Selectable(disassembly.GetAddressName(address).c_str(), &selected);
			if (selected)
			{
				a_State.m_SelectedAddress = address;
			}

			ImGui::SameLine();
			ImGui::TextUnformatted(disassembly.GetInstructionName(address).c_str());

			address += disassembly.GetInstructionSize(address);
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