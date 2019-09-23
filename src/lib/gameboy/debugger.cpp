#include <gameboy/debugger.hpp>

#include <gameboy/cpu.hpp>
#include <gameboy/device.hpp>
#include <gameboy/mmu.hpp>
#include <gameboy/ppu.hpp>

#include <limits>

using namespace Amber;
using namespace Gameboy;

Debugger::Debugger(Device& a_Device):
	m_Device(a_Device)
{
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

bool Debugger::HasBreakpoint(uint64_t a_Address) const noexcept
{
	return m_Breakpoints.count(a_Address) > 0;
}

void Debugger::SetBreakpoint(uint64_t a_Address, bool a_Enabled)
{
	if (a_Enabled)
	{
		m_Breakpoints.emplace(a_Address);
	}
	else
	{
		m_Breakpoints.erase(a_Address);
	}
}

std::unordered_set<uint64_t> Debugger::GetBreakpoints() const
{
	return m_Breakpoints;
}

bool Debugger::Run()
{
	// TODO: timing cycles
	for (; m_Cycles < PPU::FrameCycles / 4; ++m_Cycles)
	{
		Microstep();

		if (CheckBreakpoints())
		{
			return false;
		}
	}

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
	if (m_Breakpoints.size() > 0)
	{
		const uint16_t pc = m_Device.GetCPU().LoadRegister16(CPU::RegisterPC);
		if (m_Breakpoints.count(pc) > 0)
		{
			return true;
		}
	}
	
	return false;
}