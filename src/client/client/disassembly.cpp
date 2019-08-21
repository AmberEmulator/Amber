#include <client/disassembly.hpp>

#include <imgui/imgui.h>

using namespace Demu;
using namespace Client;

void Demu::Client::ShowDisassembly(const char* a_Name, DisassemblyState& a_State)
{
	auto& disassembly = *(a_State.m_Disassembly);

	ImGui::BeginChild(a_Name, ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

	uint64_t address = a_State.m_ViewAddress;
	for (size_t i = 0; i < 100; ++i)
	{
		while (!disassembly.IsValidAddress(address))
		{
			++address;
		}

		try
		{
			bool selected = address == a_State.m_SelectedAddress;
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
			break;
		}
	}

	ImGui::EndChild();
}