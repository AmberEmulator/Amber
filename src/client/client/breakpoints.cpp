#include <client/breakpoints.hpp>

#include <imgui/imgui.h>

using namespace Amber;
using namespace Client;

void Amber::Client::ShowBreakpoints(const char* a_Name, BreakpointsState& a_State)
{
	auto& debugger = *(a_State.m_Debugger);

	ImGui::BeginChild(a_Name, ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::MenuItem("New"))
		{
			ImGui::OpenPopup("NewPopup");
			a_State.m_NewAddress = 0;
		}

		if (ImGui::BeginPopupModal("NewPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::InputScalar("Address", ImGuiDataType_U64, &(a_State.m_NewAddress), nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal);
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				debugger.SetBreakpoint(a_State.m_NewAddress, true);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		const bool enabled = a_State.m_Selected.has_value();
		if (ImGui::MenuItem("Delete", nullptr, false, enabled))
		{
			debugger.SetBreakpoint(*(a_State.m_Selected), false);
			a_State.m_Selected = {};
		}

		ImGui::EndMenuBar();
	}

	const auto breakpoints = debugger.GetBreakpoints();

	for (auto&& breakpoint : breakpoints)
	{
		const auto name = debugger.GetAddressName(breakpoint);

		bool selected = a_State.m_Selected.has_value() ? *(a_State.m_Selected) == breakpoint : false;
		if (ImGui::Selectable(name.c_str(), selected))
		{
			a_State.m_Selected = breakpoint;
		}
	}

	ImGui::EndChild();
}