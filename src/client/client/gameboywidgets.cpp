#include <client/gameboywidgets.hpp>

#include <gameboy/registers.hpp>

#include <algorithm>

using namespace Demu;
using namespace Gameboy;
using namespace Client;

void Demu::Client::DrawRegisterWindow(const char* a_Name, Registers& a_Registers, bool* a_Open, ImGuiWindowFlags a_Flags)
{
	if (ImGui::Begin(a_Name, a_Open, a_Flags))
	{
		DrawRegisterWindowContent(a_Registers);
	}

	ImGui::End();
}

void Demu::Client::DrawRegisterWindowContent(Gameboy::Registers& a_Registers)
{
	const ImGuiInputTextFlags flags = ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsHexadecimal;

	ImGui::BeginGroup();

	const ImGuiStyle& style = ImGui::GetStyle();
	const float w_full = ImGui::GetContentRegionAvailWidth();
	const float w_spacing = style.ItemInnerSpacing.x;
	const float w_item_one = std::max(1.0f, std::floor((w_full - w_spacing * 7) / 8.0f));
	const float w_item_last = std::max(1.0f, std::floor(w_full - (w_item_one + w_spacing) * 7.0f));
	float offset;

	// Labels
	offset = 0.0f;
	for (uint8_t register8_index = 0; register8_index < 8; ++register8_index)
	{
		if (register8_index > 0)
		{
			offset += w_item_one + w_spacing;
			ImGui::SameLine(offset);
		}

		const float width = register8_index == 7 ? w_item_last : w_item_one;

		ImGui::PushID(register8_index);
		ImGui::PushItemWidth(width);

		const char* names[] = { "A", "F", "B", "C", "D", "E", "H", "L" };

		uint32_t value = 0;
		ImGui::LabelText("##RegisterLabel", names[register8_index]);

		ImGui::PopItemWidth();
		ImGui::PopID();
	}

	// 8 bit registers
	offset = 0.0f;
	for (uint8_t register8_index = 0; register8_index < 8; ++register8_index)
	{
		if (register8_index > 0)
		{
			offset += w_item_one + w_spacing;
			ImGui::SameLine(offset);
		}

		const float width = register8_index == 7 ? w_item_last : w_item_one;

		ImGui::PushID(register8_index);
		ImGui::PushItemWidth(width);

		uint32_t value = 0;
		ImGui::InputScalar("##Register8", ImGuiDataType_U32, &value, nullptr, nullptr, "%02X", flags);

		ImGui::PopItemWidth();
		ImGui::PopID();
	}

	// 16 bit registers
	offset = 0.0f;
	for (uint8_t register16_index = 0; register16_index < 4; ++register16_index)
	{
		if (register16_index > 0)
		{
			offset += (w_item_one + w_spacing) * 2.0f;
			ImGui::SameLine(offset);
		}

		const float width = (w_item_one + w_spacing) + (register16_index == 3 ?  w_item_last : w_item_one);

		ImGui::PushID(register16_index);
		ImGui::PushItemWidth(width);

		uint32_t value = 0;
		ImGui::InputScalar("##Register16", ImGuiDataType_U32, &value, nullptr, nullptr, "%04X", flags);

		ImGui::PopItemWidth();
		ImGui::PopID();
	}

	ImGui::EndGroup();
}