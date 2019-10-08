#include <gameboy/debugger.hpp>

#include <gameboy/cpu.hpp>
#include <gameboy/device.hpp>
#include <gameboy/event.hpp>
#include <gameboy/mmu.hpp>
#include <gameboy/ppu.hpp>

#include <iostream>
#include <limits>

using namespace Amber;
using namespace Gameboy;

Debugger::Debugger(Device& a_Device):
	m_Device(a_Device)
{
	m_Device.GetPPU().AddObserver(*this);
}

uint64_t Debugger::GetMaximumAddress() const noexcept
{
	return static_cast<uint64_t>(std::numeric_limits<uint16_t>::max());
}

bool Debugger::IsValidAddress(uint64_t a_Address) const noexcept
{
	if (a_Address > 0xFFFF)
	{
		return false;
	}

	if (a_Address == 0)
	{
		return true;
	}

	const uint16_t address = static_cast<uint16_t>(a_Address);
	auto& memory = m_Device.GetCPU().GetMemory();

	if (memory.Load8(address - 1) == Opcode::EXT)
	{
		return false;
	}

	return true;
}

uint64_t Debugger::GetInstructionSize(uint64_t a_Address) const
{
	const auto instruction = GetInstruction(a_Address);

	if (instruction.m_Instruction != Opcode::EXT)
	{
		return Opcode::GetSize(instruction.m_Instruction).value();
	}
	else
	{
		return ExtendedOpcode::GetSize(instruction.m_ExtendedInstruction).value() + 1;
	}
}

std::string Debugger::GetInstructionName(uint64_t a_Address) const
{
	const auto instruction = GetInstruction(a_Address);

	if (instruction.m_Instruction != Opcode::EXT)
	{
		return std::string(Opcode::ToString(instruction.m_Instruction).value());
	}
	else
	{
		return std::string(ExtendedOpcode::ToString(instruction.m_ExtendedInstruction).value());
	}
}

uint8_t Debugger::Load8(uint64_t a_Address) const
{
	const uint16_t address = static_cast<uint16_t>(a_Address);
	return m_Device.GetMMU().Load8(address);
}

size_t Debugger::GetEventCount() const noexcept
{
	return Event::Enums.size();
}

std::string Debugger::GetEventName(size_t a_Event) const
{
	return std::string(Event::ToString(static_cast<Event::Enum>(a_Event)).value());
}

bool Debugger::Run()
{
	// TODO: timing cycles
	m_EnteredVBlank = false;

	do
	{
		if (m_Break)
		{
			m_Break = false;
			return false;
		}

		if (!Step())
		{
			return false;
		}
	}
	while (!m_EnteredVBlank);

	m_Cycles = 0;
	return true;
}

bool Debugger::Step()
{
	while (!Microstep())
	{
	}
	return !CheckBreakpoints();
}

bool Debugger::Microstep()
{
	return m_Device.Tick();
}

bool Debugger::Reset()
{
	m_Device.Reset();
	m_Cycles = 0;
	return !CheckBreakpoints();
}

void Debugger::OnLCDModeChange(LCDMode::Enum a_From, LCDMode::Enum a_To)
{
	if (a_To == LCDMode::VBlank)
	{
		m_EnteredVBlank = true;
	}

	for (size_t breakpoint_index = 0; breakpoint_index < GetBreakpointCount(); ++breakpoint_index)
	{
		const Common::Breakpoint breakpoint = GetBreakpoint(breakpoint_index);
		auto& breakpoint_description = GetBreakpointDescription(breakpoint);

		for (size_t condition_index = 0; condition_index < breakpoint_description.GetConditionCount(); ++condition_index)
		{
			auto& condition = breakpoint_description.GetCondition(condition_index);
			if (condition.GetType() != Common::BreakpointConditionType::Event)
			{
				continue;
			}

			switch (condition.GetEvent())
			{
				case Event::VBlankBegin:
				if (a_To == LCDMode::VBlank)
				{
					m_Break = true;
					return;
				}
				break;

				case Event::VBlankEnd:
				if (a_From == LCDMode::VBlank)
				{
					m_Break = true;
					return;
				}
				break;

				case Event::OAMSearchBegin:
				if (a_To == LCDMode::OAMSearch)
				{
					m_Break = true;
					return;
				}
				break;

				case Event::OAMSearchEnd:
				if (a_From == LCDMode::OAMSearch)
				{
					m_Break = true;
					return;
				}
				break;

				case Event::PixelTransferBegin:
				if (a_To == LCDMode::PixelTransfer)
				{
					m_Break = true;

					std::cout << static_cast<size_t>(m_Device.GetPPU().GetLY()) << std::endl;
					return;
				}
				break;

				case Event::PixelTransferEnd:
				if (a_From == LCDMode::PixelTransfer)
				{
					m_Break = true;
					return;
				}
				break;

				case Event::HBlankBegin:
				if (a_To == LCDMode::HBlank)
				{
					m_Break = true;
					return;
				}
				break;

				case Event::HBlankEnd:
				if (a_From == LCDMode::HBlank)
				{
					m_Break = true;
					return;
				}
				break;
			}
		}
	}
}

void Debugger::OnBreakpointCreate(Common::Breakpoint a_Breakpoint)
{
	
}

void Debugger::OnBreakpointDestroy(Common::Breakpoint a_Breakpoint) noexcept
{

}

Debugger::InstructionInfo Debugger::GetInstruction(uint64_t a_Address) const
{
	auto& memory = m_Device.GetMMU();

	InstructionInfo instruction;

	// Cast address to correct size
	const uint16_t address = static_cast<uint16_t>(a_Address);

	// Read the instruction
	instruction.m_Instruction = static_cast<Opcode::Enum>(memory.Load8(address));

	// Check if it is an extended instruction
	if (instruction.m_Instruction == Opcode::EXT)
	{
		// Read the extended instruction
		instruction.m_ExtendedInstruction = static_cast<ExtendedOpcode::Enum>(memory.Load8(address + 1));
	}

	return instruction;
}

bool Debugger::CheckBreakpoints() const
{
	const uint16_t pc = m_Device.GetCPU().LoadRegister16(CPU::RegisterPC);
	return GetExecutionBreakpoints(pc).size() > 0;
}