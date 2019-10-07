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
			ImGui::OpenPopup("New Breakpoint");
			a_State.m_NewBreakpointDescription = {};
			a_State.m_NewBreakpointDescription.AddCondition({});
		}

		if (ImGui::BeginPopupModal("New Breakpoint", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			for (size_t breakpoint_condition_index = 0; breakpoint_condition_index < a_State.m_NewBreakpointDescription.GetConditionCount(); ++breakpoint_condition_index)
			{
				auto breakpoint_condition = a_State.m_NewBreakpointDescription.GetCondition(breakpoint_condition_index);

				switch (breakpoint_condition.GetType())
				{
					case BreakpointConditionType::Execution:
					{
						uint64_t address = breakpoint_condition.GetAddress();
						if (ImGui::InputScalar("Address", ImGuiDataType_U64, &address, nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal))
						{
							breakpoint_condition.SetAddress(address);
						}
					}
					break;
				}

				a_State.m_NewBreakpointDescription.SetCondition(breakpoint_condition_index, breakpoint_condition);
			}

			if (ImGui::Button("Add"))
			{
				a_State.m_NewBreakpointDescription.AddCondition({});
			}

			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				debugger.CreateBreakpoint(a_State.m_NewBreakpointDescription);
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