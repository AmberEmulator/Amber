#include <client/gameboywidgets.hpp>

#include <gameboy/registers.hpp>

#include <algorithm>

using namespace Amber;
using namespace Gameboy;
using namespace Client;

void Amber::Client::DrawRegisterWindow(const char* a_Name, Registers& a_Registers, bool* a_Open, ImGuiWindowFlags a_Flags)
{
	if (ImGui::Begin(a_Name, a_Open, a_Flags))
	{
		DrawRegisterWindowContent(a_Registers);
	}

	ImGui::End();
}

void Amber::Client::DrawRegisterWindowContent(Gameboy::Registers& a_Registers)
{
	ImGui::BeginGroup();

	const ImGuiStyle& style = ImGui::GetStyle();

	const char* names[] = { "AF", "BC", "DE", "HL", "SP", "PC" };
	for (uint8_t register16_index = 0; register16_index < 6; ++register16_index)
	{
		ImGui::PushID(register16_index);

		const float input_width = ImGui::CalcTextSize("0000").x + style.FramePadding.x * 2.0f;
		const ImGuiInputTextFlags flags = ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsHexadecimal;

		ImGui::PushItemWidth(input_width);

		uint32_t value = a_Registers.GetRegister16(register16_index);
		if (ImGui::InputScalar(names[register16_index], ImGuiDataType_U32, &value, nullptr, nullptr, "%04X", flags))
		{
			a_Registers.SetRegister16(static_cast<uint16_t>(register16_index), static_cast<uint16_t>(value));
		}

		ImGui::PopItemWidth();

		ImGui::PopID();
	}

	if (bool value = a_Registers.GetZero(); ImGui::Checkbox("Zero", &value))
	{
		a_Registers.SetZero(value);
	}
	if (bool value = a_Registers.GetSubtract(); ImGui::Checkbox("Subtract", &value))
	{
		a_Registers.SetSubtract(value);
	}
	if (bool value = a_Registers.GetHalfCarry(); ImGui::Checkbox("Half Carry", &value))
	{
		a_Registers.SetHalfCarry(value);
	}
	if (bool value = a_Registers.GetCarry(); ImGui::Checkbox("Carry", &value))
	{
		a_Registers.SetCarry(value);
	}

	ImGui::EndGroup();
}