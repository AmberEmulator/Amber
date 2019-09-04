#include <gameboy/cpu.hpp>

#include <gameboy/instructionbuilder.hpp>

#include <cassert>

using namespace Amber;
using namespace Common;
using namespace Gameboy;

CPU::CPU(Memory16& a_Memory):
	m_Memory(a_Memory)
{
	InstructionBuilder<Opcode::Enum> instruction_builder;
	instruction_builder.Resize(256);

	// Misc instructions
	instruction_builder.Begin(Opcode::NOP);

	// 8-bit load register to register
	instruction_builder.Begin(Opcode::LD_A_A, &CPU::LD_r_r<RegisterIndexA, RegisterIndexA>);
	instruction_builder.Begin(Opcode::LD_A_B, &CPU::LD_r_r<RegisterIndexA, RegisterIndexB>);
	instruction_builder.Begin(Opcode::LD_A_C, &CPU::LD_r_r<RegisterIndexA, RegisterIndexC>);
	instruction_builder.Begin(Opcode::LD_A_D, &CPU::LD_r_r<RegisterIndexA, RegisterIndexD>);
	instruction_builder.Begin(Opcode::LD_A_E, &CPU::LD_r_r<RegisterIndexA, RegisterIndexE>);
	instruction_builder.Begin(Opcode::LD_A_H, &CPU::LD_r_r<RegisterIndexA, RegisterIndexH>);
	instruction_builder.Begin(Opcode::LD_A_L, &CPU::LD_r_r<RegisterIndexA, RegisterIndexL>);

	instruction_builder.Begin(Opcode::LD_B_A, &CPU::LD_r_r<RegisterIndexB, RegisterIndexA>);
	instruction_builder.Begin(Opcode::LD_B_B, &CPU::LD_r_r<RegisterIndexB, RegisterIndexB>);
	instruction_builder.Begin(Opcode::LD_B_C, &CPU::LD_r_r<RegisterIndexB, RegisterIndexC>);
	instruction_builder.Begin(Opcode::LD_B_D, &CPU::LD_r_r<RegisterIndexB, RegisterIndexD>);
	instruction_builder.Begin(Opcode::LD_B_E, &CPU::LD_r_r<RegisterIndexB, RegisterIndexE>);
	instruction_builder.Begin(Opcode::LD_B_H, &CPU::LD_r_r<RegisterIndexB, RegisterIndexH>);
	instruction_builder.Begin(Opcode::LD_B_L, &CPU::LD_r_r<RegisterIndexB, RegisterIndexL>);

	instruction_builder.Begin(Opcode::LD_C_A, &CPU::LD_r_r<RegisterIndexC, RegisterIndexA>);
	instruction_builder.Begin(Opcode::LD_C_B, &CPU::LD_r_r<RegisterIndexC, RegisterIndexB>);
	instruction_builder.Begin(Opcode::LD_C_C, &CPU::LD_r_r<RegisterIndexC, RegisterIndexC>);
	instruction_builder.Begin(Opcode::LD_C_D, &CPU::LD_r_r<RegisterIndexC, RegisterIndexD>);
	instruction_builder.Begin(Opcode::LD_C_E, &CPU::LD_r_r<RegisterIndexC, RegisterIndexE>);
	instruction_builder.Begin(Opcode::LD_C_H, &CPU::LD_r_r<RegisterIndexC, RegisterIndexH>);
	instruction_builder.Begin(Opcode::LD_C_L, &CPU::LD_r_r<RegisterIndexC, RegisterIndexL>);

	instruction_builder.Begin(Opcode::LD_D_A, &CPU::LD_r_r<RegisterIndexD, RegisterIndexA>);
	instruction_builder.Begin(Opcode::LD_D_B, &CPU::LD_r_r<RegisterIndexD, RegisterIndexB>);
	instruction_builder.Begin(Opcode::LD_D_C, &CPU::LD_r_r<RegisterIndexD, RegisterIndexC>);
	instruction_builder.Begin(Opcode::LD_D_D, &CPU::LD_r_r<RegisterIndexD, RegisterIndexD>);
	instruction_builder.Begin(Opcode::LD_D_E, &CPU::LD_r_r<RegisterIndexD, RegisterIndexE>);
	instruction_builder.Begin(Opcode::LD_D_H, &CPU::LD_r_r<RegisterIndexD, RegisterIndexH>);
	instruction_builder.Begin(Opcode::LD_D_L, &CPU::LD_r_r<RegisterIndexD, RegisterIndexL>);

	instruction_builder.Begin(Opcode::LD_E_A, &CPU::LD_r_r<RegisterIndexE, RegisterIndexA>);
	instruction_builder.Begin(Opcode::LD_E_B, &CPU::LD_r_r<RegisterIndexE, RegisterIndexB>);
	instruction_builder.Begin(Opcode::LD_E_C, &CPU::LD_r_r<RegisterIndexE, RegisterIndexC>);
	instruction_builder.Begin(Opcode::LD_E_D, &CPU::LD_r_r<RegisterIndexE, RegisterIndexD>);
	instruction_builder.Begin(Opcode::LD_E_E, &CPU::LD_r_r<RegisterIndexE, RegisterIndexE>);
	instruction_builder.Begin(Opcode::LD_E_H, &CPU::LD_r_r<RegisterIndexE, RegisterIndexH>);
	instruction_builder.Begin(Opcode::LD_E_L, &CPU::LD_r_r<RegisterIndexE, RegisterIndexL>);

	instruction_builder.Begin(Opcode::LD_H_A, &CPU::LD_r_r<RegisterIndexH, RegisterIndexA>);
	instruction_builder.Begin(Opcode::LD_H_B, &CPU::LD_r_r<RegisterIndexH, RegisterIndexB>);
	instruction_builder.Begin(Opcode::LD_H_C, &CPU::LD_r_r<RegisterIndexH, RegisterIndexC>);
	instruction_builder.Begin(Opcode::LD_H_D, &CPU::LD_r_r<RegisterIndexH, RegisterIndexD>);
	instruction_builder.Begin(Opcode::LD_H_E, &CPU::LD_r_r<RegisterIndexH, RegisterIndexE>);
	instruction_builder.Begin(Opcode::LD_H_H, &CPU::LD_r_r<RegisterIndexH, RegisterIndexH>);
	instruction_builder.Begin(Opcode::LD_H_L, &CPU::LD_r_r<RegisterIndexH, RegisterIndexL>);

	instruction_builder.Begin(Opcode::LD_L_A, &CPU::LD_r_r<RegisterIndexL, RegisterIndexA>);
	instruction_builder.Begin(Opcode::LD_L_B, &CPU::LD_r_r<RegisterIndexL, RegisterIndexB>);
	instruction_builder.Begin(Opcode::LD_L_C, &CPU::LD_r_r<RegisterIndexL, RegisterIndexC>);
	instruction_builder.Begin(Opcode::LD_L_D, &CPU::LD_r_r<RegisterIndexL, RegisterIndexD>);
	instruction_builder.Begin(Opcode::LD_L_E, &CPU::LD_r_r<RegisterIndexL, RegisterIndexE>);
	instruction_builder.Begin(Opcode::LD_L_H, &CPU::LD_r_r<RegisterIndexL, RegisterIndexH>);
	instruction_builder.Begin(Opcode::LD_L_L, &CPU::LD_r_r<RegisterIndexL, RegisterIndexL>);

	// 8-bit load next byte to register
	instruction_builder.Begin(Opcode::LD_A_n).Cycle(&CPU::LD_c_n<0>, &CPU::LD_r_c<RegisterIndexA, 0>);
	instruction_builder.Begin(Opcode::LD_B_n).Cycle(&CPU::LD_c_n<0>, &CPU::LD_r_c<RegisterIndexB, 0>);
	instruction_builder.Begin(Opcode::LD_C_n).Cycle(&CPU::LD_c_n<0>, &CPU::LD_r_c<RegisterIndexC, 0>);
	instruction_builder.Begin(Opcode::LD_D_n).Cycle(&CPU::LD_c_n<0>, &CPU::LD_r_c<RegisterIndexD, 0>);
	instruction_builder.Begin(Opcode::LD_E_n).Cycle(&CPU::LD_c_n<0>, &CPU::LD_r_c<RegisterIndexE, 0>);
	instruction_builder.Begin(Opcode::LD_H_n).Cycle(&CPU::LD_c_n<0>, &CPU::LD_r_c<RegisterIndexH, 0>);
	instruction_builder.Begin(Opcode::LD_L_n).Cycle(&CPU::LD_c_n<0>, &CPU::LD_r_c<RegisterIndexL, 0>);

	// 8-bit load byte at address to register
	instruction_builder.Begin(Opcode::LD_A_aBC).Cycle(&CPU::LD_c_arr<0, RegisterIndexBC>, &CPU::LD_r_c<RegisterIndexA, 0>);
	instruction_builder.Begin(Opcode::LD_A_aDE).Cycle(&CPU::LD_c_arr<0, RegisterIndexDE>, &CPU::LD_r_c<RegisterIndexA, 0>);
	instruction_builder.Begin(Opcode::LD_A_aHL).Cycle(&CPU::LD_c_arr<0, RegisterIndexHL>, &CPU::LD_r_c<RegisterIndexA, 0>);
	instruction_builder.Begin(Opcode::LD_B_aHL).Cycle(&CPU::LD_c_arr<0, RegisterIndexHL>, &CPU::LD_r_c<RegisterIndexB, 0>);
	instruction_builder.Begin(Opcode::LD_C_aHL).Cycle(&CPU::LD_c_arr<0, RegisterIndexHL>, &CPU::LD_r_c<RegisterIndexC, 0>);
	instruction_builder.Begin(Opcode::LD_D_aHL).Cycle(&CPU::LD_c_arr<0, RegisterIndexHL>, &CPU::LD_r_c<RegisterIndexD, 0>);
	instruction_builder.Begin(Opcode::LD_E_aHL).Cycle(&CPU::LD_c_arr<0, RegisterIndexHL>, &CPU::LD_r_c<RegisterIndexE, 0>);
	instruction_builder.Begin(Opcode::LD_H_aHL).Cycle(&CPU::LD_c_arr<0, RegisterIndexHL>, &CPU::LD_r_c<RegisterIndexH, 0>);
	instruction_builder.Begin(Opcode::LD_L_aHL).Cycle(&CPU::LD_c_arr<0, RegisterIndexHL>, &CPU::LD_r_c<RegisterIndexL, 0>);

	// 8-bit load misc to register
	instruction_builder.Begin(Opcode::LD_A_ann).Cycle(&CPU::LD_c_n<1>).Cycle(&CPU::LD_c_n<0>).Cycle(&CPU::LD_c_acc<0>, &CPU::LD_r_c<RegisterIndexA, 0>);
	instruction_builder.Begin(Opcode::LD_A_aFFC).Cycle(&CPU::LD_c_r<0, RegisterIndexC>, &CPU::LD_cc_FFc<0>, &CPU::LD_c_acc<0>, &CPU::LD_r_c<RegisterIndexA, 0>);
	instruction_builder.Begin(Opcode::LD_A_aFFn).Cycle(&CPU::LD_c_n<0>).Cycle(&CPU::LD_cc_FFc<0>, &CPU::LD_c_acc<0>, &CPU::LD_r_c<RegisterIndexA, 0>);

	// 8 bit load register to address
	instruction_builder.Begin(Opcode::LD_aBC_A).Cycle(&CPU::LD_arr_r<RegisterIndexBC, RegisterIndexA>);
	instruction_builder.Begin(Opcode::LD_aDE_A).Cycle(&CPU::LD_arr_r<RegisterIndexDE, RegisterIndexA>);
	instruction_builder.Begin(Opcode::LD_aHL_A).Cycle(&CPU::LD_arr_r<RegisterIndexHL, RegisterIndexA>);
	instruction_builder.Begin(Opcode::LD_aHL_B).Cycle(&CPU::LD_arr_r<RegisterIndexHL, RegisterIndexB>);
	instruction_builder.Begin(Opcode::LD_aHL_C).Cycle(&CPU::LD_arr_r<RegisterIndexHL, RegisterIndexC>);
	instruction_builder.Begin(Opcode::LD_aHL_D).Cycle(&CPU::LD_arr_r<RegisterIndexHL, RegisterIndexD>);
	instruction_builder.Begin(Opcode::LD_aHL_E).Cycle(&CPU::LD_arr_r<RegisterIndexHL, RegisterIndexE>);
	instruction_builder.Begin(Opcode::LD_aHL_H).Cycle(&CPU::LD_arr_r<RegisterIndexHL, RegisterIndexH>);
	instruction_builder.Begin(Opcode::LD_aHL_L).Cycle(&CPU::LD_arr_r<RegisterIndexHL, RegisterIndexL>);

	// 8 bit load next byte to address
	instruction_builder.Begin(Opcode::LD_aHL_n).Cycle(&CPU::LD_c_n<0>).Cycle(&CPU::LD_arr_c<RegisterIndexHL, 0>);

	// 8-bit load misc to address
	instruction_builder.Begin(Opcode::LD_ann_A).Cycle(&CPU::LD_c_n<1>).Cycle(&CPU::LD_c_n<0>).Cycle(&CPU::LD_acc_r<RegisterIndexA>);
	instruction_builder.Begin(Opcode::LD_aFFC_A).Cycle(&CPU::LD_c_r<0, RegisterIndexC>, &CPU::LD_cc_FFc<0>, &CPU::LD_acc_r<RegisterIndexA>);
	instruction_builder.Begin(Opcode::LD_aFFn_A).Cycle(&CPU::LD_c_n<0>, &CPU::LD_cc_FFc<0>).Cycle(&CPU::LD_acc_r<RegisterIndexA>);

	// 8-bit load and increment/decrement
	instruction_builder.Begin(Opcode::LDI_A_aHL).Cycle(&CPU::LD_c_arr<0, RegisterIndexHL>, &CPU::LD_r_c<RegisterIndexA, 0>, &CPU::UnaryOp_rr<RegisterIndexHL, &CPU::IncrementWord>);
	instruction_builder.Begin(Opcode::LDD_A_aHL).Cycle(&CPU::LD_c_arr<0, RegisterIndexHL>, &CPU::LD_r_c<RegisterIndexA, 0>, &CPU::UnaryOp_rr<RegisterIndexHL, &CPU::DecrementWord>);
	instruction_builder.Begin(Opcode::LDI_aHL_A).Cycle(&CPU::LD_arr_r<RegisterIndexHL, RegisterIndexA>, &CPU::UnaryOp_rr<RegisterIndexHL, &CPU::IncrementWord>);
	instruction_builder.Begin(Opcode::LDD_aHL_A).Cycle(&CPU::LD_arr_r<RegisterIndexHL, RegisterIndexA>, &CPU::UnaryOp_rr<RegisterIndexHL, &CPU::DecrementWord>);

	// 16-bit loads
	instruction_builder.Begin(Opcode::LD_BC_nn).Cycle(&CPU::LD_c_n<1>).Cycle(&CPU::LD_c_n<0>, &CPU::LD_rr_cc<RegisterIndexBC>);
	instruction_builder.Begin(Opcode::LD_DE_nn).Cycle(&CPU::LD_c_n<1>).Cycle(&CPU::LD_c_n<0>, &CPU::LD_rr_cc<RegisterIndexDE>);
	instruction_builder.Begin(Opcode::LD_HL_nn).Cycle(&CPU::LD_c_n<1>).Cycle(&CPU::LD_c_n<0>, &CPU::LD_rr_cc<RegisterIndexHL>);
	instruction_builder.Begin(Opcode::LD_HL_SPn).Cycle(&CPU::LD_c_n<0>).Cycle(&CPU::LD_cc_rrc<RegisterIndexSP, 0>, &CPU::LD_rr_cc<RegisterIndexHL>);
	instruction_builder.Begin(Opcode::LD_SP_nn).Cycle(&CPU::LD_c_n<1>).Cycle(&CPU::LD_c_n<0>, &CPU::LD_rr_cc<RegisterIndexSP>);
	instruction_builder.Begin(Opcode::LD_SP_HL).Cycle(&CPU::LD_rr_rr< RegisterIndexSP, RegisterIndexHL>);
	instruction_builder.Begin(Opcode::LD_ann_SP).Cycle(&CPU::LD_c_n<1>).Cycle(&CPU::LD_c_n<0>).Cycle(&CPU::LD_acc_rr<RegisterIndexSP>);
	
	// Build instruction sets
	m_Instructions = instruction_builder.Build();

	// Reset CPU
	Reset();
}

Memory16& CPU::GetMemory() const noexcept
{
	return m_Memory;
}

bool CPU::Tick()
{
	m_OpBreak = false;
	while (!m_OpBreak)
	{
		const MicroOp op = PopOp();
		CallOp(op);
	}
	return m_OpDone;
}

void CPU::Reset()
{
	m_OpCount = 0;
	PushOp(&CPU::DecodeInstruction);
	SetPC(0);
}

uint8_t CPU::PeekNextByte() const noexcept
{
	return m_Memory.Load8(GetPC());
}

uint16_t CPU::PeekNextWord() const noexcept
{
	return m_Memory.Load16(GetPC());
}

uint8_t CPU::ReadNextByte() noexcept
{
	const uint8_t byte = PeekNextByte();
	SetPC(GetPC() + 1);
	return byte;
}

uint16_t CPU::ReadNextWord() noexcept
{
	const uint16_t word = PeekNextWord();
	SetPC(GetPC() + 2);
	return word;
}

void CPU::PushOp(MicroOp a_Op)
{
	assert(m_OpCount < std::size(m_MicroOps));

	const size_t index = (m_CurrentOp + m_OpCount) % std::size(m_MicroOps);
	m_MicroOps[index] = a_Op;
	++m_OpCount;
}

CPU::MicroOp CPU::PopOp()
{
	assert(m_OpCount > 0);

	const MicroOp op = m_MicroOps[m_CurrentOp];
	m_CurrentOp = (m_CurrentOp + 1) % std::size(m_MicroOps);
	--m_OpCount;

	return op;
}

void CPU::CallOp(MicroOp a_Op)
{
	(this->*a_Op)();
}

template <bool Carry>
uint8_t CPU::AddByte(uint8_t a_Left, uint8_t a_Right) noexcept
{
	if constexpr (Carry)
	{
		if (GetCarry())
		{
			++a_Right;
		}
	}

	const uint16_t result = static_cast<uint16_t>(a_Left) + static_cast<uint16_t>(a_Right);

	SetZero((result & 0xFF) == 0);
	SetSubtract(false);
	SetHalfCarry((((a_Left & 0xF) + (a_Right & 0xF)) & 0xF0) != 0);
	SetCarry((result & 0xFF00) != 0);

	return static_cast<uint8_t>(result);
}

template <bool Carry>
uint8_t CPU::SubtractByte(uint8_t a_Left, uint8_t a_Right) noexcept
{
	if constexpr (Carry)
	{
		if (GetCarry())
		{
			++a_Right;
		}
	}

	const uint8_t value = AddByte<false>(a_Left, (~a_Right) + 1);

	SetSubtract(true);
	SetHalfCarry(!GetHalfCarry());
	SetCarry(!GetCarry());

	return value;
}

uint8_t CPU::IncrementByte(uint8_t a_Value) noexcept
{
	SetZero(a_Value == 0xFF);
	SetSubtract(false);
	SetHalfCarry((a_Value & 0xF) == 0xF);
	// Carry flag is not affected by increments

	return ++a_Value;
}

uint8_t CPU::DecrementByte(uint8_t a_Value) noexcept
{
	SetZero(a_Value == 0x01);
	SetSubtract(true);
	SetHalfCarry((a_Value & 0xF) == 0);
	// Carry flag is not affected by decrements

	return --a_Value;
}

uint16_t CPU::IncrementWord(uint16_t a_Value) noexcept
{
	return ++a_Value;
}

uint16_t CPU::DecrementWord(uint16_t a_Value) noexcept
{
	return --a_Value;
}

uint8_t CPU::ANDByte(uint8_t a_Left, uint8_t a_Right) noexcept
{
	const uint8_t result = (a_Left & a_Right);

	SetZero(result == 0);
	SetSubtract(false);
	SetHalfCarry(true);
	SetCarry(false);

	return result;
}

uint8_t CPU::ORByte(uint8_t a_Left, uint8_t a_Right) noexcept
{
	const uint8_t result = (a_Left | a_Right);

	SetZero(result == 0);
	SetSubtract(false);
	SetHalfCarry(false);
	SetCarry(false);

	return result;
}

uint8_t CPU::XORByte(uint8_t a_Left, uint8_t a_Right) noexcept
{
	const uint8_t result = (a_Left ^ a_Right);

	SetZero(result == 0);
	SetSubtract(false);
	SetHalfCarry(false);
	SetCarry(false);

	return result;
}

uint8_t CPU::SwapByte(uint8_t a_Value) noexcept
{
	const uint8_t result = ((a_Value & 0x0F) << 4) | ((a_Value & 0xF0) >> 4);

	SetZero(result == 0);
	SetSubtract(false);
	SetHalfCarry(false);
	SetCarry(false);

	return result;
}

uint8_t CPU::RotateLeftByte(uint8_t a_Value) noexcept
{
	const uint8_t result = (a_Value << 1) | (a_Value >> 7);

	SetZero(result == 0);
	SetSubtract(false);
	SetHalfCarry(false);
	SetCarry((a_Value & 0x80) != 0);

	return result;
}

uint8_t CPU::RotateLeftThroughCarryByte(uint8_t a_Value) noexcept
{
	const uint8_t result = (a_Value << 1) | (GetCarry() ? 0x01 : 0x00);

	SetZero(result == 0);
	SetSubtract(false);
	SetHalfCarry(false);
	SetCarry((a_Value & 0x80) != 0);

	return result;
}

uint8_t CPU::RotateRightByte(uint8_t a_Value) noexcept
{
	const uint8_t result = (a_Value >> 1) | (a_Value << 7);

	SetZero(result == 0);
	SetSubtract(false);
	SetHalfCarry(false);
	SetCarry((a_Value & 0x01) != 0);

	return result;
}

uint8_t CPU::RotateRightThroughCarryByte(uint8_t a_Value) noexcept
{
	const uint8_t result = (a_Value >> 1) | (GetCarry() ? 0x80 : 0x00);

	SetZero(result == 0);
	SetSubtract(false);
	SetHalfCarry(false);
	SetCarry((a_Value & 0x01) != 0);

	return result;
}

template <uint8_t Destination, CPU::UnaryOp8 Op, bool Store>
void CPU::UnaryOp_r() noexcept
{
	const uint8_t destination_value = GetRegister8(Destination);
	const uint8_t result = (this->*Op)(destination_value);
	if constexpr (Store)
	{
		SetRegister8(Destination, result);
	}
}

template <uint8_t Destination, CPU::UnaryOp8 Op, bool Store>
void CPU::UnaryOp_arr() noexcept
{
	const uint16_t address = GetRegister16(Destination);
	const uint8_t destination_value = m_Memory.Load8(address);
	const uint8_t result = (this->*Op)(destination_value);
	if constexpr (Store)
	{
		m_Memory.Store8(address, result);
	}
}

template <uint8_t Destination, CPU::UnaryOp16 Op, bool Store>
void CPU::UnaryOp_rr() noexcept
{
	const uint16_t destination_value = GetRegister16(Destination);
	const uint16_t result = (this->*Op)(destination_value);
	if constexpr (Store)
	{
		SetRegister16(Destination, result);
	}
}

template <uint8_t Destination, CPU::BinaryOp8 Op, bool Store>
void CPU::BinaryOp_r_x(uint8_t a_Value) noexcept
{
	const uint8_t destination_value = GetRegister8(Destination);
	const uint8_t result = (this->*Op)(destination_value, a_Value);
	if constexpr (Store)
	{
		SetRegister8(Destination, result);
	}
}

template <uint8_t Destination, CPU::BinaryOp8 Op, bool Store>
void CPU::BinaryOp_r_n() noexcept
{
	const uint8_t value = ReadNextByte();
	BinaryOp_r_x<Destination, Op, Store>(value);
}

template <uint8_t Destination, uint8_t Source, CPU::BinaryOp8 Op, bool Store>
void CPU::BinaryOp_r_r() noexcept
{
	const uint8_t value = GetRegister8(Source);
	BinaryOp_r_x<Destination, Op, Store>(value);
}

template <uint8_t Destination, uint8_t Source, CPU::BinaryOp8 Op, bool Store>
void CPU::BinaryOp_r_arr() noexcept
{
	const uint16_t address = GetRegister16(Source);
	const uint8_t value = m_Memory.Load8(address);
	BinaryOp_r_x<Destination, Op, Store>(value);
}

void CPU::DecodeInstruction()
{
	m_OpDone = false;

	const auto opcode = static_cast<Opcode::Enum>(ReadNextByte());
	const size_t instruction_size = m_Instructions->GetInstructionSize(opcode);
	const MicroOp* const ops = m_Instructions->GetInstructionOps(opcode);

	for (size_t i = 0; i < instruction_size; ++i)
	{
		PushOp(ops[i]);
	}

	PushOp(&CPU::DecodeInstruction);
}

void CPU::Break()
{
	m_OpBreak = true;
}

void CPU::Done()
{
	m_OpDone = true;
}

template <uint8_t Destination>
void CPU::LD_r_x(uint8_t a_Value)
{
	SetRegister8(Destination, a_Value);
}

template <uint8_t Destination, uint8_t Source>
void CPU::LD_r_r()
{
	const uint8_t value = GetRegister8(Source);
	LD_r_x<Destination>(value);
}

template <uint8_t Destination, uint8_t Source>
void CPU::LD_r_c()
{
	const uint8_t value = m_OpCache.GetByte<Source>();
	LD_r_x<Destination>(value);
}

void CPU::LD_axx_x(uint16_t a_Address, uint8_t a_Value)
{
	m_Memory.Store8(a_Address, a_Value);
}

template <uint8_t Destination>
void CPU::LD_arr_x(uint8_t a_Value)
{
	const uint16_t address = GetRegister16(Destination);
	LD_axx_x(address, a_Value);
}

template <uint8_t Destination, uint8_t Source>
void CPU::LD_arr_r()
{
	const uint8_t value = GetRegister8(Source);
	LD_arr_x<Destination>(value);
}

template <uint8_t Destination, uint8_t Source>
void CPU::LD_arr_c()
{
	const uint8_t value = m_OpCache.GetByte<Source>();
	LD_arr_x<Destination>(value);
}

template <uint8_t Destination>
void CPU::LD_rr_xx(uint16_t a_Value)
{
	SetRegister16(Destination, a_Value);
}

template <uint8_t Destination>
void CPU::LD_rr_cc()
{
	const uint16_t value = m_OpCache.GetWord();
	LD_rr_xx<Destination>(value);
}

template <uint8_t Destination, uint8_t Source>
void CPU::LD_rr_rr()
{
	const uint16_t value = GetRegister16(Source);
	LD_rr_xx<Destination>(value);
}

template <uint8_t Destination>
void CPU::LD_c_x(uint8_t a_Value)
{
	m_OpCache.SetByte<Destination>(a_Value);
}

template <uint8_t Destination, uint8_t Source>
void CPU::LD_c_r()
{
	const uint8_t value = GetRegister8(Source);
	LD_c_x<Destination>(value);
}

template <uint8_t Destination>
void CPU::LD_c_n()
{
	const uint8_t value = ReadNextByte();
	LD_c_x<Destination>(value);
}

template <uint8_t Destination>
void CPU::LD_c_axx(uint16_t a_Address)
{
	const uint8_t value = m_Memory.Load8(a_Address);
	LD_c_x<Destination>(value);
}

template <uint8_t Destination, uint8_t Source>
void CPU::LD_c_arr()
{
	const uint16_t address = GetRegister16(Source);
	LD_c_axx<Destination>(address);
}

template <uint8_t Destination>
void CPU::LD_c_acc()
{
	const uint16_t address = m_OpCache.GetWord();
	LD_c_axx<Destination>(address);
}

void CPU::LD_cc_xx(uint16_t a_Value)
{
	m_OpCache.SetWord(a_Value);
}

template <uint8_t Source>
void CPU::LD_cc_rr()
{
	const uint16_t value = GetRegister16(Source);
	LD_cc_xx(value);
}

void CPU::LD_cc_FFx(uint8_t a_Offset)
{
	const uint16_t value = 0xFF00_u16 | a_Offset;
	LD_cc_xx(value);
}

template <uint8_t Source>
void CPU::LD_cc_FFc()
{
	const uint8_t offset = m_OpCache.GetByte<Source>();
	LD_cc_FFx(offset);
}

template <uint8_t Source>
void CPU::LD_cc_rrx(uint8_t a_Offset)
{
	const uint16_t value = GetRegister16(Source) | a_Offset;
	LD_cc_xx(value);
}

template <uint8_t SourceRegister, uint8_t SourceCache>
void CPU::LD_cc_rrc()
{
	const uint8_t offset = m_OpCache.GetByte<SourceCache>();
	LD_cc_rrx<SourceRegister>(offset);
}

void CPU::LD_acc_x(uint8_t a_Value)
{
	const uint16_t address = m_OpCache.GetWord();
	LD_axx_x(address, a_Value);
}

template <uint8_t Source>
void CPU::LD_acc_r()
{
	const uint8_t value = GetRegister8(Source);
	LD_acc_x(value);
}

void CPU::LD_axx_xx(uint16_t a_Address, uint16_t a_Value)
{
	m_Memory.Store16(a_Address, a_Value);
}

void CPU::LD_acc_xx(uint16_t a_Value)
{
	const uint16_t address = m_OpCache.GetWord();
	LD_axx_xx(address, a_Value);
}

template <uint8_t Source>
void CPU::LD_acc_rr()
{
	const uint16_t value = GetRegister16(Source);
	LD_acc_xx(value);
}