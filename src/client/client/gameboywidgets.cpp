#include <client/gameboywidgets.hpp>

#include <gameboy/cpu.hpp>

#include <algorithm>

using namespace Amber;
using namespace Gameboy;
using namespace Client;

void Amber::Client::DrawRegisterWindow(const char* a_Name, CPU& a_CPU, bool* a_Open, ImGuiWindowFlags a_Flags)
{
	if (ImGui::Begin(a_Name, a_Open, a_Flags))
	{
		DrawRegisterWindowContent(a_CPU);
	}

	ImGui::End();
}

void Amber::Client::DrawRegisterWindowContent(CPU& a_CPU)
{
	ImGui::BeginGroup();

	const ImGuiStyle& style = ImGui::GetStyle();

	const char* names[] = { "AF", "BC", "DE", "HL", "SP", "PC", "XY", "ZW" };
	for (uint8_t register16_index = 0; register16_index < sizeof(names) / sizeof(names[0]); ++register16_index)
	{
		ImGui::PushID(register16_index);
		if (register16_index % 2 != 0)
		{
			ImGui::SameLine();
		}

		const float input_width = ImGui::CalcTextSize("0000").x + style.FramePadding.x * 2.0f;
		const ImGuiInputTextFlags flags = ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsHexadecimal;

		ImGui::PushItemWidth(input_width);

		uint32_t value = a_CPU.LoadRegister16(register16_index);
		if (ImGui::InputScalar(names[register16_index], ImGuiDataType_U32, &value, nullptr, nullptr, "%04X", flags))
		{
			a_CPU.StoreRegister16(static_cast<uint16_t>(register16_index), static_cast<uint16_t>(value));
		}

		ImGui::PopItemWidth();

		ImGui::PopID();
	}

	if (bool value = a_CPU.LoadFlag(CPU::FlagZero); ImGui::Checkbox("Zero", &value))
	{
		a_CPU.StoreFlag(CPU::FlagZero, value);
	}
	if (bool value = a_CPU.LoadFlag(CPU::FlagSubtract); ImGui::Checkbox("Subtract", &value))
	{
		a_CPU.StoreFlag(CPU::FlagSubtract, value);
	}
	if (bool value = a_CPU.LoadFlag(CPU::FlagHalfCarry); ImGui::Checkbox("Half Carry", &value))
	{
		a_CPU.StoreFlag(CPU::FlagHalfCarry, value);
	}
	if (bool value = a_CPU.LoadFlag(CPU::FlagCarry); ImGui::Checkbox("Carry", &value))
	{
		a_CPU.StoreFlag(CPU::FlagCarry, value);
	}

	ImGui::EndGroup();
}