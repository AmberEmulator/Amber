#include <catch2/catch.hpp>

#include <gameboy/cpu.hpp>

#include <common/rom.hpp>

using namespace Amber;
using namespace Common;
using namespace Gameboy;

struct CPUTestFixture
{
	static constexpr uint16_t ROMSize = 16;

	CPUTestFixture():
		m_ROM(ROMSize, 1),
		m_CPU(m_ROM, GameboyType::Classic)
	{
		auto& registers = m_CPU.GetRegisters();

		registers = {};
		registers.SetZero(GENERATE(false, true));
		registers.SetSubtract(GENERATE(false, true));
		registers.SetHalfCarry(GENERATE(false, true));
		registers.SetCarry(GENERATE(false, true));
	}

	void ValidateRegisters()
	{
		REQUIRE(m_CPU.GetRegisters() == m_ExpectedRegisters);
	}

	void ValidateROM()
	{
		REQUIRE(std::memcmp(m_ROM.GetData(), m_ExpectedROM, ROMSize) == 0);
	}

	void Validate()
	{
		ValidateRegisters();
		ValidateROM();
	}

	void MirrorExpected()
	{
		m_ExpectedRegisters = m_CPU.GetRegisters();
		std::memcpy(m_ExpectedROM, m_ROM.GetData(), ROMSize);
	}

	void ExecuteNextInstruction()
	{
		const auto executed_instruction = m_CPU.ExecuteNextInstruction();

		uint16_t instruction_cycles;
		uint16_t instruction_size;
		if (executed_instruction.m_Instruction != Opcode::EXT)
		{
			instruction_cycles = Opcode::GetCycles(executed_instruction.m_Instruction).value();
			instruction_size = Opcode::GetSize(executed_instruction.m_Instruction).value();
		}
		else
		{
			instruction_cycles = ExtendedOpcode::GetCycles(executed_instruction.m_ExtendedInstruction).value();
			instruction_size = ExtendedOpcode::GetSize(executed_instruction.m_ExtendedInstruction).value();
		}

		m_ExpectedRegisters.SetPC(m_ExpectedRegisters.GetPC() + instruction_size);
	}

	ROM<uint16_t, false> m_ROM;
	CPU m_CPU;

	Registers m_ExpectedRegisters;
	uint8_t m_ExpectedROM[ROMSize] = {};
};

#if NDEBUG
#define GENERATE_CPU_BYTE GENERATE(range<uint8_t>(0, 255))
#else
#define GENERATE_CPU_BYTE static_cast<uint8_t>(GENERATE(values<uint8_t>({ 0, 1, 2, 7, 8, 16, 128, 254, 254, 255})))
#endif

namespace
{
	void Add(Registers& a_Registers, uint8_t a_Left, uint8_t a_Right)
	{
		const uint16_t result = static_cast<uint16_t>(a_Left) + static_cast<uint16_t>(a_Right);

		a_Registers.SetA(static_cast<uint8_t>(result));
		a_Registers.SetZero((result & 0xFF) == 0);
		a_Registers.SetSubtract(false);
		a_Registers.SetHalfCarry((((a_Left & 0xF) + (a_Right & 0xF)) & 0xF0) != 0);
		a_Registers.SetCarry((result & 0xFF00) != 0);
	}
}

TEST_CASE_METHOD(CPUTestFixture, "ADD A,r", "[CPU][ADD]")
{
	struct InstructionInfo
	{
		Opcode::Enum m_Instruction;
		uint8_t m_Register;
	};

	const InstructionInfo instruction_info = GENERATE(values<InstructionInfo>({
		{ Instruction::ADD_A_A, Registers::RegisterIndexA },
		{ Instruction::ADD_A_B, Registers::RegisterIndexB },
		{ Instruction::ADD_A_C, Registers::RegisterIndexC },
		{ Instruction::ADD_A_D, Registers::RegisterIndexD },
		{ Instruction::ADD_A_E, Registers::RegisterIndexE },
		{ Instruction::ADD_A_H, Registers::RegisterIndexH },
		{ Instruction::ADD_A_L, Registers::RegisterIndexL }
	}));

	m_ROM.GetData()[0] = instruction_info.m_Instruction;

	auto& registers = m_CPU.GetRegisters();
	registers.SetA(GENERATE_CPU_BYTE);
	registers.SetRegister8(instruction_info.m_Register, GENERATE_CPU_BYTE);

	const uint8_t left = registers.GetA();
	const uint8_t right = registers.GetRegister8(instruction_info.m_Register);

	MirrorExpected();
	ExecuteNextInstruction();

	Add(m_ExpectedRegisters, left, right);

	Validate();
}

TEST_CASE_METHOD(CPUTestFixture, "ADD A,n", "[CPU][ADD]")
{
	const uint8_t left = GENERATE_CPU_BYTE;
	const uint8_t right = GENERATE_CPU_BYTE;

	m_ROM.GetData()[0] = Opcode::ADD_A_n;
	m_ROM.GetData()[1] = right;

	auto& registers = m_CPU.GetRegisters();
	registers.SetA(left);

	MirrorExpected();
	ExecuteNextInstruction();

	Add(m_ExpectedRegisters, left, right);

	Validate();
}

TEST_CASE_METHOD(CPUTestFixture, "ADD A,aHL", "[CPU][ADD]")
{
	const uint8_t left = GENERATE_CPU_BYTE;
	const uint8_t right = GENERATE_CPU_BYTE;

	m_ROM.GetData()[0] = Opcode::ADD_A_aHL;
	m_ROM.GetData()[ROMSize - 1] = right;

	auto& registers = m_CPU.GetRegisters();
	registers.SetA(left);
	registers.SetHL(ROMSize - 1);

	MirrorExpected();
	ExecuteNextInstruction();

	Add(m_ExpectedRegisters, left, right);

	Validate();
}