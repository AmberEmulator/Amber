#include <client/breakpoints.hpp>

#include <imgui/imgui.h>

using namespace Amber;
using namespace Common;
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
				BreakpointCondition breakpoint_condition;
				breakpoint_condition.SetType(BreakpointConditionType::Execution);
				breakpoint_condition.SetAddress(a_State.m_NewAddress);

				BreakpointDescription breakpoint_description;
				breakpoint_description.AddCondition(breakpoint_condition);

				debugger.CreateBreakpoint(breakpoint_description);
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

		if (ImGui::MenuItem("Delete", nullptr, false, a_State.m_Selected != nullptr))
		{
			debugger.DestroyBreakpoint(a_State.m_Selected);
			a_State.m_Selected = nullptr;
		}

		ImGui::EndMenuBar();
	}

	for (size_t breakpoint_index = 0; breakpoint_index < debugger.GetBreakpointCount(); ++breakpoint_index)
	{
		ImGui::PushID(breakpoint_index);

		const auto breakpoint = debugger.GetBreakpoint(breakpoint_index);

		bool selected = a_State.m_Selected == breakpoint;
		if (ImGui::Selectable("Foo", selected))
		{
			a_State.m_Selected = breakpoint;
		}

		ImGui::PopID();
	}

	ImGui::EndChild();
}