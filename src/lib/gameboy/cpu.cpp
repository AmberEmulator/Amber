#include <gameboy/cpu.hpp>

using namespace Demu;
using namespace Common;
using namespace Gameboy;

CPU::CPU(Memory16& a_Memory, GameboyType::Enum a_GameboyType):
	m_GameboyType(a_GameboyType),
	m_Memory(a_Memory)
{
	// Initialize instructions
	for (size_t i = 0; i < 256; ++i)
	{
		m_Instructions[i] = &CPU::NotImplemented;
		m_ExtendedInstructions[i] = &CPU::NotImplemented;
	}

	// Misc instructions
	m_Instructions[Instruction::NOP] = &CPU::NOP;

	// 8-bit load instructions
	m_Instructions[Instruction::LD_A_A]    = &CPU::LD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexA>;
	m_Instructions[Instruction::LD_A_B]    = &CPU::LD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexB>;
	m_Instructions[Instruction::LD_A_C]    = &CPU::LD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexC>;
	m_Instructions[Instruction::LD_A_D]    = &CPU::LD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexD>;
	m_Instructions[Instruction::LD_A_E]    = &CPU::LD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexE>;
	m_Instructions[Instruction::LD_A_H]    = &CPU::LD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexH>;
	m_Instructions[Instruction::LD_A_L]    = &CPU::LD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexL>;
	m_Instructions[Instruction::LD_A_n]    = &CPU::LD_r_n<Registers::RegisterIndexA>;
	m_Instructions[Instruction::LD_A_aBC]  = &CPU::LD_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexBC>;
	m_Instructions[Instruction::LD_A_aDE]  = &CPU::LD_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexDE>;
	m_Instructions[Instruction::LD_A_aHL]  = &CPU::LD_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL>;
	m_Instructions[Instruction::LD_A_ann]  = &CPU::LD_r_ann<Registers::RegisterIndexA>;
	m_Instructions[Instruction::LD_A_aFFC] = &CPU::LD_r_aFFr<Registers::RegisterIndexA, Registers::RegisterIndexC>;
	m_Instructions[Instruction::LD_A_aFFn] = &CPU::LD_r_aFFn<Registers::RegisterIndexA>;

	m_Instructions[Instruction::LD_B_A]   = &CPU::LD_r_r<Registers::RegisterIndexB, Registers::RegisterIndexA>;
	m_Instructions[Instruction::LD_B_B]   = &CPU::LD_r_r<Registers::RegisterIndexB, Registers::RegisterIndexB>;
	m_Instructions[Instruction::LD_B_C]   = &CPU::LD_r_r<Registers::RegisterIndexB, Registers::RegisterIndexC>;
	m_Instructions[Instruction::LD_B_D]   = &CPU::LD_r_r<Registers::RegisterIndexB, Registers::RegisterIndexD>;
	m_Instructions[Instruction::LD_B_E]   = &CPU::LD_r_r<Registers::RegisterIndexB, Registers::RegisterIndexE>;
	m_Instructions[Instruction::LD_B_H]   = &CPU::LD_r_r<Registers::RegisterIndexB, Registers::RegisterIndexH>;
	m_Instructions[Instruction::LD_B_L]   = &CPU::LD_r_r<Registers::RegisterIndexB, Registers::RegisterIndexL>;
	m_Instructions[Instruction::LD_B_n]   = &CPU::LD_r_n<Registers::RegisterIndexB>;
	m_Instructions[Instruction::LD_B_aHL] = &CPU::LD_r_arr<Registers::RegisterIndexB, Registers::RegisterIndexHL>;

	m_Instructions[Instruction::LD_C_A]   = &CPU::LD_r_r<Registers::RegisterIndexC, Registers::RegisterIndexA>;
	m_Instructions[Instruction::LD_C_B]   = &CPU::LD_r_r<Registers::RegisterIndexC, Registers::RegisterIndexB>;
	m_Instructions[Instruction::LD_C_C]   = &CPU::LD_r_r<Registers::RegisterIndexC, Registers::RegisterIndexC>;
	m_Instructions[Instruction::LD_C_D]   = &CPU::LD_r_r<Registers::RegisterIndexC, Registers::RegisterIndexD>;
	m_Instructions[Instruction::LD_C_E]   = &CPU::LD_r_r<Registers::RegisterIndexC, Registers::RegisterIndexE>;
	m_Instructions[Instruction::LD_C_H]   = &CPU::LD_r_r<Registers::RegisterIndexC, Registers::RegisterIndexH>;
	m_Instructions[Instruction::LD_C_L]   = &CPU::LD_r_r<Registers::RegisterIndexC, Registers::RegisterIndexL>;
	m_Instructions[Instruction::LD_C_n]   = &CPU::LD_r_n<Registers::RegisterIndexC>;
	m_Instructions[Instruction::LD_C_aHL] = &CPU::LD_r_arr<Registers::RegisterIndexC, Registers::RegisterIndexHL>;

	m_Instructions[Instruction::LD_D_A]   = &CPU::LD_r_r<Registers::RegisterIndexD, Registers::RegisterIndexA>;
	m_Instructions[Instruction::LD_D_B]   = &CPU::LD_r_r<Registers::RegisterIndexD, Registers::RegisterIndexB>;
	m_Instructions[Instruction::LD_D_C]   = &CPU::LD_r_r<Registers::RegisterIndexD, Registers::RegisterIndexC>;
	m_Instructions[Instruction::LD_D_D]   = &CPU::LD_r_r<Registers::RegisterIndexD, Registers::RegisterIndexD>;
	m_Instructions[Instruction::LD_D_E]   = &CPU::LD_r_r<Registers::RegisterIndexD, Registers::RegisterIndexE>;
	m_Instructions[Instruction::LD_D_H]   = &CPU::LD_r_r<Registers::RegisterIndexD, Registers::RegisterIndexH>;
	m_Instructions[Instruction::LD_D_L]   = &CPU::LD_r_r<Registers::RegisterIndexD, Registers::RegisterIndexL>;
	m_Instructions[Instruction::LD_D_n]   = &CPU::LD_r_n<Registers::RegisterIndexD>;
	m_Instructions[Instruction::LD_D_aHL] = &CPU::LD_r_arr<Registers::RegisterIndexD, Registers::RegisterIndexHL>;

	m_Instructions[Instruction::LD_E_A]   = &CPU::LD_r_r<Registers::RegisterIndexE, Registers::RegisterIndexA>;
	m_Instructions[Instruction::LD_E_B]   = &CPU::LD_r_r<Registers::RegisterIndexE, Registers::RegisterIndexB>;
	m_Instructions[Instruction::LD_E_C]   = &CPU::LD_r_r<Registers::RegisterIndexE, Registers::RegisterIndexC>;
	m_Instructions[Instruction::LD_E_D]   = &CPU::LD_r_r<Registers::RegisterIndexE, Registers::RegisterIndexD>;
	m_Instructions[Instruction::LD_E_E]   = &CPU::LD_r_r<Registers::RegisterIndexE, Registers::RegisterIndexE>;
	m_Instructions[Instruction::LD_E_H]   = &CPU::LD_r_r<Registers::RegisterIndexE, Registers::RegisterIndexH>;
	m_Instructions[Instruction::LD_E_L]   = &CPU::LD_r_r<Registers::RegisterIndexE, Registers::RegisterIndexL>;
	m_Instructions[Instruction::LD_E_n]   = &CPU::LD_r_n<Registers::RegisterIndexE>;
	m_Instructions[Instruction::LD_E_aHL] = &CPU::LD_r_arr<Registers::RegisterIndexE, Registers::RegisterIndexHL>;

	m_Instructions[Instruction::LD_H_A]   = &CPU::LD_r_r<Registers::RegisterIndexH, Registers::RegisterIndexA>;
	m_Instructions[Instruction::LD_H_B]   = &CPU::LD_r_r<Registers::RegisterIndexH, Registers::RegisterIndexB>;
	m_Instructions[Instruction::LD_H_C]   = &CPU::LD_r_r<Registers::RegisterIndexH, Registers::RegisterIndexC>;
	m_Instructions[Instruction::LD_H_D]   = &CPU::LD_r_r<Registers::RegisterIndexH, Registers::RegisterIndexD>;
	m_Instructions[Instruction::LD_H_E]   = &CPU::LD_r_r<Registers::RegisterIndexH, Registers::RegisterIndexE>;
	m_Instructions[Instruction::LD_H_H]   = &CPU::LD_r_r<Registers::RegisterIndexH, Registers::RegisterIndexH>;
	m_Instructions[Instruction::LD_H_L]   = &CPU::LD_r_r<Registers::RegisterIndexH, Registers::RegisterIndexL>;
	m_Instructions[Instruction::LD_H_n]   = &CPU::LD_r_n<Registers::RegisterIndexH>;
	m_Instructions[Instruction::LD_H_aHL] = &CPU::LD_r_arr<Registers::RegisterIndexH, Registers::RegisterIndexHL>;

	m_Instructions[Instruction::LD_L_A]   = &CPU::LD_r_r<Registers::RegisterIndexL, Registers::RegisterIndexA>;
	m_Instructions[Instruction::LD_L_B]   = &CPU::LD_r_r<Registers::RegisterIndexL, Registers::RegisterIndexB>;
	m_Instructions[Instruction::LD_L_C]   = &CPU::LD_r_r<Registers::RegisterIndexL, Registers::RegisterIndexC>;
	m_Instructions[Instruction::LD_L_D]   = &CPU::LD_r_r<Registers::RegisterIndexL, Registers::RegisterIndexD>;
	m_Instructions[Instruction::LD_L_E]   = &CPU::LD_r_r<Registers::RegisterIndexL, Registers::RegisterIndexE>;
	m_Instructions[Instruction::LD_L_H]   = &CPU::LD_r_r<Registers::RegisterIndexL, Registers::RegisterIndexH>;
	m_Instructions[Instruction::LD_L_L]   = &CPU::LD_r_r<Registers::RegisterIndexL, Registers::RegisterIndexL>;
	m_Instructions[Instruction::LD_L_n]   = &CPU::LD_r_n<Registers::RegisterIndexL>;
	m_Instructions[Instruction::LD_L_aHL] = &CPU::LD_r_arr<Registers::RegisterIndexL, Registers::RegisterIndexHL>;

	m_Instructions[Instruction::LD_aHL_A] = &CPU::LD_arr_r<Registers::RegisterIndexHL, Registers::RegisterIndexA>;
	m_Instructions[Instruction::LD_aHL_B] = &CPU::LD_arr_r<Registers::RegisterIndexHL, Registers::RegisterIndexB>;
	m_Instructions[Instruction::LD_aHL_C] = &CPU::LD_arr_r<Registers::RegisterIndexHL, Registers::RegisterIndexC>;
	m_Instructions[Instruction::LD_aHL_D] = &CPU::LD_arr_r<Registers::RegisterIndexHL, Registers::RegisterIndexD>;
	m_Instructions[Instruction::LD_aHL_E] = &CPU::LD_arr_r<Registers::RegisterIndexHL, Registers::RegisterIndexE>;
	m_Instructions[Instruction::LD_aHL_H] = &CPU::LD_arr_r<Registers::RegisterIndexHL, Registers::RegisterIndexH>;
	m_Instructions[Instruction::LD_aHL_L] = &CPU::LD_arr_r<Registers::RegisterIndexHL, Registers::RegisterIndexL>;
	m_Instructions[Instruction::LD_aHL_n] = &CPU::LD_arr_n<Registers::RegisterIndexHL>;
	
	m_Instructions[Instruction::LD_aBC_A] = &CPU::LD_arr_r<Registers::RegisterIndexBC, Registers::RegisterIndexA>;
	m_Instructions[Instruction::LD_aDE_A] = &CPU::LD_arr_r<Registers::RegisterIndexDE, Registers::RegisterIndexA>;
	m_Instructions[Instruction::LD_ann_A] = &CPU::LD_ann_r<Registers::RegisterIndexA>;
	m_Instructions[Instruction::LD_aFFC_A] = &CPU::LD_aFFr_r<Registers::RegisterIndexC, Registers::RegisterIndexA>;
	m_Instructions[Instruction::LD_aFFn_A] = &CPU::LD_aFFn_r<Registers::RegisterIndexA>;

	m_Instructions[Instruction::LDI_A_aHL] = &CPU::Increment16<Registers::RegisterIndexHL, &CPU::LD_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL>>;
	m_Instructions[Instruction::LDD_A_aHL] = &CPU::Decrement16<Registers::RegisterIndexHL, & CPU::LD_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL>>;
	m_Instructions[Instruction::LDI_aHL_A] = &CPU::Increment16<Registers::RegisterIndexHL, & CPU::LD_arr_r<Registers::RegisterIndexHL, Registers::RegisterIndexA>>;
	m_Instructions[Instruction::LDD_aHL_A] = &CPU::Decrement16<Registers::RegisterIndexHL, & CPU::LD_arr_r<Registers::RegisterIndexHL, Registers::RegisterIndexA>>;

	// 16-bit load instructions
	m_Instructions[Instruction::LD_BC_nn]  = &CPU::LD_rr_nn<Registers::RegisterIndexBC>;
	m_Instructions[Instruction::LD_DE_nn]  = &CPU::LD_rr_nn<Registers::RegisterIndexDE>;
	m_Instructions[Instruction::LD_HL_nn]  = &CPU::LD_rr_nn<Registers::RegisterIndexHL>;
	m_Instructions[Instruction::LD_HL_SPn] = &CPU::LD_rr_rrn<Registers::RegisterIndexHL, Registers::RegisterIndexSP>;
	m_Instructions[Instruction::LD_SP_nn]  = &CPU::LD_rr_nn<Registers::RegisterIndexSP>;
	m_Instructions[Instruction::LD_SP_HL]  = &CPU::LD_rr_rr<Registers::RegisterIndexSP, Registers::RegisterIndexHL>;
	m_Instructions[Instruction::LD_ann_SP] = &CPU::LD_ann_rr<Registers::RegisterIndexSP>;

	// 8-bit add instructions
	m_Instructions[Instruction::ADD_A_A]   = &CPU::ADD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexA, false>;
	m_Instructions[Instruction::ADD_A_B]   = &CPU::ADD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexB, false>;
	m_Instructions[Instruction::ADD_A_C]   = &CPU::ADD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexC, false>;
	m_Instructions[Instruction::ADD_A_D]   = &CPU::ADD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexD, false>;
	m_Instructions[Instruction::ADD_A_E]   = &CPU::ADD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexE, false>;
	m_Instructions[Instruction::ADD_A_H]   = &CPU::ADD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexH, false>;
	m_Instructions[Instruction::ADD_A_L]   = &CPU::ADD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexL, false>;
	m_Instructions[Instruction::ADD_A_n]   = &CPU::ADD_r_n<Registers::RegisterIndexA, false>;
	m_Instructions[Instruction::ADD_A_aHL] = &CPU::ADD_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL, false>;

	// 16-bit add instructions
	m_Instructions[Instruction::ADD_HL_BC] = &CPU::ADD_rr_rr<Registers::RegisterIndexHL, Registers::RegisterIndexBC>;
	m_Instructions[Instruction::ADD_HL_DE] = &CPU::ADD_rr_rr<Registers::RegisterIndexHL, Registers::RegisterIndexDE>;
	m_Instructions[Instruction::ADD_HL_HL] = &CPU::ADD_rr_rr<Registers::RegisterIndexHL, Registers::RegisterIndexHL>;
	m_Instructions[Instruction::ADD_HL_SP] = &CPU::ADD_rr_rr<Registers::RegisterIndexHL, Registers::RegisterIndexSP>;
	m_Instructions[Instruction::ADD_SP_n]  = &CPU::ADD_rr_n<Registers::RegisterIndexSP>;

	// 8-bit add + carry instructions
	m_Instructions[Instruction::ADC_A_A]   = &CPU::ADD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexA, true>;
	m_Instructions[Instruction::ADC_A_B]   = &CPU::ADD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexB, true>;
	m_Instructions[Instruction::ADC_A_C]   = &CPU::ADD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexC, true>;
	m_Instructions[Instruction::ADC_A_D]   = &CPU::ADD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexD, true>;
	m_Instructions[Instruction::ADC_A_E]   = &CPU::ADD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexE, true>;
	m_Instructions[Instruction::ADC_A_H]   = &CPU::ADD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexH, true>;
	m_Instructions[Instruction::ADC_A_L]   = &CPU::ADD_r_r<Registers::RegisterIndexA, Registers::RegisterIndexL, true>;
	m_Instructions[Instruction::ADC_A_n]   = &CPU::ADD_r_n<Registers::RegisterIndexA, true>;
	m_Instructions[Instruction::ADC_A_aHL] = &CPU::ADD_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL, true>;

	// Absolute jump instructions
	m_Instructions[Instruction::JP_nn]    = &CPU::JP_nn;
	m_Instructions[Instruction::JP_NZ_nn] = &CPU::Conditional<Registers::FlagIndexZero, false, &CPU::JP_nn>;
	m_Instructions[Instruction::JP_Z_nn]  = &CPU::Conditional<Registers::FlagIndexZero, true, &CPU::JP_nn>;
	m_Instructions[Instruction::JP_NC_nn] = &CPU::Conditional<Registers::FlagIndexCarry, false, &CPU::JP_nn>;
	m_Instructions[Instruction::JP_C_nn]  = &CPU::Conditional<Registers::FlagIndexCarry, true, &CPU::JP_nn>;
	m_Instructions[Instruction::JP_HL]    = &CPU::JP_rr<Registers::RegisterIndexHL>;

	// Relative jump instructions
	m_Instructions[Instruction::JR_n]    = &CPU::JR_n;
	m_Instructions[Instruction::JR_NZ_n] = &CPU::Conditional<Registers::FlagIndexZero, false, &CPU::JR_n>;
	m_Instructions[Instruction::JR_Z_n]  = &CPU::Conditional<Registers::FlagIndexZero, true, &CPU::JR_n>;
	m_Instructions[Instruction::JR_NC_n] = &CPU::Conditional<Registers::FlagIndexCarry, false, &CPU::JR_n>;
	m_Instructions[Instruction::JR_C_n]  = &CPU::Conditional<Registers::FlagIndexCarry, true, &CPU::JR_n>;
}

GameboyType::Enum CPU::GetGameboyType() const noexcept
{
	return m_GameboyType;
}

Memory16& CPU::GetMemory() noexcept
{
	return const_cast<Memory16&>(static_cast<const CPU*>(this)->GetMemory());
}

const Memory16& CPU::GetMemory() const noexcept
{
	return m_Memory;
}

Registers& CPU::GetRegisters() noexcept
{
	return const_cast<Registers&>(static_cast<const CPU*>(this)->GetRegisters());
}

const Registers& CPU::GetRegisters() const noexcept
{
	return m_Registers;
}

void CPU::Reset()
{
	// Initialize registers
	m_Registers.SetAF(0x01B0);
	m_Registers.SetBC(0x0013);
	m_Registers.SetDE(0x00D8);
	m_Registers.SetHL(0x014D);
	m_Registers.SetPC(EntryPoint);
	m_Registers.SetSP(0xFFFE);

	m_Memory.Store8(0xFF05, 0x00); //TIMA
	m_Memory.Store8(0xFF06, 0x00); //TMA
	m_Memory.Store8(0xFF07, 0x00); //TAC
	m_Memory.Store8(0xFF10, 0x80); //NR10
	m_Memory.Store8(0xFF11, 0xBF); //NR11
	m_Memory.Store8(0xFF12, 0xF3); //NR12
	m_Memory.Store8(0xFF14, 0xBF); //NR14
	m_Memory.Store8(0xFF16, 0x3F); //NR21
	m_Memory.Store8(0xFF17, 0x00); //NR22
	m_Memory.Store8(0xFF19, 0xBF); //NR24
	m_Memory.Store8(0xFF1A, 0x7F); //NR30
	m_Memory.Store8(0xFF1B, 0xFF); //NR31
	m_Memory.Store8(0xFF1C, 0x9F); //NR32
	m_Memory.Store8(0xFF1E, 0xBF); //NR33
	m_Memory.Store8(0xFF20, 0xFF); //NR41
	m_Memory.Store8(0xFF21, 0x00); //NR42
	m_Memory.Store8(0xFF22, 0x00); //NR43
	m_Memory.Store8(0xFF23, 0xBF); //NR30
	m_Memory.Store8(0xFF24, 0x77); //NR50
	m_Memory.Store8(0xFF25, 0xF3); //NR51

	// $F1 - GB, $F0 - SGB; NR52
	if (GetGameboyType() == GameboyType::Super)
	{
		m_Memory.Store8(0xFF26, 0xF0);
	}
	else
	{

		m_Memory.Store8(0xFF26, 0xF1);
	}

	m_Memory.Store8(0xFF40, 0x91); //LCDC
	m_Memory.Store8(0xFF42, 0x00); //SCY
	m_Memory.Store8(0xFF43, 0x00); //SCX
	m_Memory.Store8(0xFF45, 0x00); //LYC
	m_Memory.Store8(0xFF47, 0xFC); //BGP
	m_Memory.Store8(0xFF48, 0xFF); //OBP0
	m_Memory.Store8(0xFF49, 0xFF); //OBP1
	m_Memory.Store8(0xFF4A, 0x00); //WY
	m_Memory.Store8(0xFF4B, 0x00); //WX
	m_Memory.Store8(0xFFFF, 0x00); //IE
}

CPU::ExecutedInstruction CPU::ExecuteNextInstruction()
{
	ExecutedInstruction instruction;

	instruction.m_Instruction = static_cast<Instruction::Enum>(ReadNextByte());
	instruction.m_ExtendedInstruction = instruction.m_Instruction == Instruction::EXT ? static_cast<ExtendedInstruction::Enum>(0) : static_cast<ExtendedInstruction::Enum>(PeekNextByte());

	ExecuteInstruction(instruction.m_Instruction);

	return instruction;
}

void CPU::ExecuteInstruction(Instruction::Enum a_Instruction)
{
	(this->*(m_Instructions[a_Instruction]))();
	m_CycleCount += Instruction::GetCycles(a_Instruction).value();
}

void CPU::ExecuteNextExtendedInstruction()
{
	ExecuteExtendedInstruction(static_cast<ExtendedInstruction::Enum>(ReadNextByte()));
}

void CPU::ExecuteExtendedInstruction(ExtendedInstruction::Enum a_Instruction)
{
	(this->*(m_ExtendedInstructions[a_Instruction]))();
	m_CycleCount += ExtendedInstruction::GetCycleCount(a_Instruction).value();
}

uint8_t CPU::PeekNextByte() const noexcept
{
	return m_Memory.Load8(m_Registers.GetPC());
}

uint16_t CPU::PeekNextWord() const noexcept
{
	return m_Memory.Load16(m_Registers.GetPC());
}

uint8_t CPU::ReadNextByte() noexcept
{
	const uint8_t byte = PeekNextByte();
	m_Registers.SetPC(m_Registers.GetPC() + 1);
	return byte;
}

uint16_t CPU::ReadNextWord() noexcept
{
	const uint16_t word = PeekNextWord();
	m_Registers.SetPC(m_Registers.GetPC() + 2);
	return word;
}

uint8_t CPU::AddByte(uint8_t a_Left, uint8_t a_Right) noexcept
{
	const uint16_t result = static_cast<uint16_t>(a_Left) + static_cast<uint16_t>(a_Right);

	m_Registers.SetZero((result & 0xFF) == 0);
	m_Registers.SetSubtract(false);
	m_Registers.SetHalfCarry((((a_Left & 0xF) + (a_Right & 0xF)) & 0xF0) != 0);
	m_Registers.SetCarry((result & 0xFF00) != 0);

	return static_cast<uint8_t>(result);
}

uint8_t CPU::SubtractByte(uint8_t a_Left, uint8_t a_Right) noexcept
{
	const uint8_t value = AddByte(a_Left, (~a_Right) + 1);

	m_Registers.SetSubtract(true);
	m_Registers.SetHalfCarry(!m_Registers.GetHalfCarry());
	m_Registers.SetCarry(!m_Registers.GetCarry());

	return value;
}

uint8_t CPU::IncrementByte(uint8_t a_Value) noexcept
{
	m_Registers.SetZero(a_Value == 0xFF);
	m_Registers.SetSubtract(false);
	m_Registers.SetHalfCarry((a_Value & 0xF) == 0xF);
	// Carry flag is not affected by increments

	return ++a_Value;
}

uint8_t CPU::DecrementByte(uint8_t a_Value) noexcept
{
	m_Registers.SetZero(a_Value == 0x01);
	m_Registers.SetSubtract(true);
	m_Registers.SetHalfCarry((a_Value & 0xF) == 0);
	// Carry flag is not affected by decrements

	return --a_Value;
}

uint8_t CPU::ANDByte(uint8_t a_Left, uint8_t a_Right) noexcept
{
	const uint8_t result = (a_Left & a_Right);

	m_Registers.SetZero(result == 0);
	m_Registers.SetSubtract(false);
	m_Registers.SetHalfCarry(true);
	m_Registers.SetCarry(false);

	return result;
}

uint8_t CPU::ORByte(uint8_t a_Left, uint8_t a_Right) noexcept
{
	const uint8_t result = (a_Left | a_Right);

	m_Registers.SetZero(result == 0);
	m_Registers.SetSubtract(false);
	m_Registers.SetHalfCarry(false);
	m_Registers.SetCarry(false);

	return result;
}

uint8_t CPU::XORByte(uint8_t a_Left, uint8_t a_Right) noexcept
{
	const uint8_t result = (a_Left ^ a_Right);

	m_Registers.SetZero(result == 0);
	m_Registers.SetSubtract(false);
	m_Registers.SetHalfCarry(false);
	m_Registers.SetCarry(false);

	return result;
}

template <uint8_t Flag, bool Set, CPU::InstructionCallback Callback>
void CPU::Conditional() noexcept
{
	if (m_Registers.GetFlag(Flag) == Set)
	{
		(this->*Callback)();
	}
}

template <uint8_t Register, CPU::InstructionCallback Callback>
void CPU::Increment16() noexcept
{
	const uint16_t value = m_Registers.GetRegister16(Register) + 1_u16;
	m_Registers.SetRegister16(Register, value);
}

template <uint8_t Register, CPU::InstructionCallback Callback>
void CPU::Decrement16() noexcept
{
	const uint16_t value = m_Registers.GetRegister16(Register) - 1_u16;
	m_Registers.SetRegister16(Register, value);
}

void CPU::NotImplemented() noexcept
{
	assert(false && "This instruction has not been implemented.");
}

void CPU::NOP() noexcept
{
}

template <uint8_t Destination>
void CPU::LD_r_x(uint8_t a_Value) noexcept
{
	m_Registers.SetRegister8(Destination, a_Value);
}

template <uint8_t Destination>
void CPU::LD_r_n() noexcept
{
	const uint8_t value = ReadNextByte();
	LD_r_x<Destination>(value);
}

template <uint8_t Destination, uint8_t Source>
void CPU::LD_r_r() noexcept
{
	const uint8_t value = m_Registers.GetRegister8(Source);
	LD_r_x<Destination>(value);
}

template <uint8_t Destination>
void CPU::LD_r_axx(uint16_t a_Address) noexcept
{
	const uint8_t value = m_Memory.Load8(a_Address);
	LD_r_x<Destination>(value);
}

template <uint8_t Destination>
void CPU::LD_r_ann() noexcept
{
	const uint16_t address = ReadNextWord();
	LD_r_axx<Destination>(address);
}

template <uint8_t Destination, uint8_t Source>
void CPU::LD_r_arr() noexcept
{
	const uint16_t address = m_Registers.GetRegister16(Source);
	LD_r_axx<Destination>(address);
}

template <uint8_t Destination>
void CPU::LD_r_aFFx(uint8_t a_Offset) noexcept
{
	const uint16_t address = 0xFF00_u16 | a_Offset;
	LD_r_axx<Destination>(address);
}

template <uint8_t Destination>
void CPU::LD_r_aFFn() noexcept
{
	const uint8_t offset = ReadNextByte();
	LD_r_aFFx<Destination>(offset);
}

template <uint8_t Destination, uint8_t Source>
void CPU::LD_r_aFFr() noexcept
{
	const uint8_t offset = m_Registers.GetRegister8(Source);
	LD_r_aFFx<Destination>(offset);
}

void CPU::LD_axx_x(uint16_t a_Address, uint8_t a_Value)
{
	m_Memory.Store8(a_Address, a_Value);
}

template <uint8_t Source>
void CPU::LD_axx_r(uint16_t a_Address) noexcept
{
	const uint8_t value = m_Registers.GetRegister8(Source);
	LD_axx_x(a_Address, value);
}

template <uint8_t Destination>
void CPU::LD_arr_x(uint8_t a_Value) noexcept
{
	const uint16_t address = m_Registers.GetRegister16(Destination);
	LD_axx_x(address, a_Value);
}

template <uint8_t Destination>
void CPU::LD_arr_n() noexcept
{
	const uint8_t value = ReadNextByte();
	LD_arr_x<Destination>(value);
}

template <uint8_t Destination, uint8_t Source>
void CPU::LD_arr_r() noexcept
{
	const uint8_t value = m_Registers.GetRegister8(Source);
	LD_arr_x<Destination>(value);
}

template <uint8_t Source>
void CPU::LD_ann_x(uint8_t a_Value) noexcept
{
	const uint16_t address = ReadNextWord();
	LD_axx_x(address, a_Value);
}

template <uint8_t Source>
void CPU::LD_ann_r() noexcept
{
	const uint8_t value = m_Registers.GetRegister8(Source);
	LD_ann_x<Source>(value);
}

template <uint8_t Source>
void CPU::LD_aFFx_r(uint8_t a_Offset) noexcept
{
	const uint16_t address = 0xFF00_u16 | a_Offset;
	LD_axx_r<Source>(address);
}

template <uint8_t Source>
void CPU::LD_aFFn_r() noexcept
{
	const uint8_t offset = ReadNextByte();
	LD_aFFx_r<Source>(offset);
}

template <uint8_t Destination, uint8_t Source>
void CPU::LD_aFFr_r() noexcept
{
	const uint8_t offset = m_Registers.GetRegister8(Destination);
	LD_aFFx_r<Source>(offset);
}

template <uint8_t Destination>
void CPU::LD_rr_xx(uint16_t a_Value) noexcept
{
	m_Registers.SetRegister16(Destination, a_Value);
}

template <uint8_t Destination>
void CPU::LD_rr_nn() noexcept
{
	const uint16_t value = ReadNextWord();
	LD_rr_xx<Destination>(value);
}

template <uint8_t Destination, uint8_t Source>
void CPU::LD_rr_rr() noexcept
{
	const uint16_t value = m_Registers.GetRegister16(Source);
	LD_rr_xx<Destination>(value);
}

template <uint8_t Destination, uint8_t Source>
void CPU::LD_rr_rrn() noexcept
{
	const uint16_t value = m_Registers.GetRegister16(Source) + ReadNextByte();
	LD_rr_xx<Destination>(value);
}

template <uint8_t Source>
void CPU::LD_xx_rr(uint16_t a_Address) noexcept
{
	const uint16_t value = m_Registers.GetRegister16(Source);
	m_Memory.Store16(a_Address, value);
}

template <uint8_t Source>
void CPU::LD_ann_rr() noexcept
{
	const uint16_t address = ReadNextWord();
	LD_xx_rr<Source>(address);
}

template <uint8_t Destination, bool Carry>
void CPU::ADD_r_x(uint8_t a_Value) noexcept
{
	const uint8_t destination_value = m_Registers.GetRegister8(Destination);
	const bool carry = m_Registers.GetCarry();
	uint8_t result = AddByte(destination_value, a_Value);
	if constexpr(Carry)
	{
		if (carry)
		{
			++result;
		}
	}
	
	m_Registers.SetRegister8(Destination, result);
}

template <uint8_t Destination, bool Carry>
void CPU::ADD_r_n() noexcept
{
	const uint8_t value = ReadNextByte();
	ADD_r_x<Destination, Carry>(value);
}

template <uint8_t Destination, uint8_t Source, bool Carry>
void CPU::ADD_r_r() noexcept
{
	const uint8_t value = m_Registers.GetRegister8(Source);
	ADD_r_x<Destination, Carry>(value);
}

template <uint8_t Destination, uint8_t Source, bool Carry>
void CPU::ADD_r_arr() noexcept
{
	const uint16_t address = m_Registers.GetRegister16(Source);
	const uint8_t value = m_Memory.Load8(address);
	ADD_r_x<Destination, Carry>(value);
}

template <uint8_t Destination>
void CPU::ADD_rr_xx(uint16_t a_Value) noexcept
{
	m_Registers.SetRegister16(Destination, a_Value);
}

template <uint8_t Destination>
void CPU::ADD_rr_n() noexcept
{
	const uint8_t value = ReadNextByte();
	ADD_rr_xx<Destination>(value);
}

template <uint8_t Destination, uint8_t Source>
void CPU::ADD_rr_rr() noexcept
{
	const uint16_t value = m_Registers.GetRegister16(Source);
	ADD_rr_xx<Destination>(value);
}

template <uint8_t Destination, bool Carry>
void CPU::SUB_r_x(uint8_t a_Value) noexcept
{
	SUB_r
}

template <uint8_t Destination, bool Carry>
void CPU::SUB_r_n() noexcept
{
	const uint8_t value = ReadNextByte();
	SUB_r_x<Destination, Carry>(value);
}

template <uint8_t Destination, uint8_t Source, bool Carry>
void CPU::SUB_r_r() noexcept
{
	const uint8_t value = m_Registers.GetRegister8(Source);
	SUB_r_x<Destination, Carry>(value);
}

template <uint8_t Destination, uint8_t Source, bool Carry>
void CPU::SUB_r_arr() noexcept
{
	const uint16_t address = m_Registers.GetRegister16(Source);
	const uint8_t value = m_Memory.Load8(address);
	SUB_r_x<Destination, Carry>(value);
}

void CPU::JP_xx(uint16_t a_Address) noexcept
{
	m_Registers.SetPC(a_Address);
}

void CPU::JP_nn() noexcept
{
	JP_xx(ReadNextWord());
}

template <uint8_t Register>
void CPU::JP_rr() noexcept
{
	JP_xx(m_Registers.GetRegister16(Register));
}

void CPU::JR_x(uint8_t a_Offset) noexcept
{
	JP_xx(m_Registers.GetPC() + a_Offset);
}

void CPU::JR_n() noexcept
{
	JR_x(ReadNextByte());
}