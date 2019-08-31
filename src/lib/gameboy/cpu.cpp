#include <gameboy/cpu.hpp>

using namespace Amber;
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
	m_Instructions[Instruction::DI]  = &CPU::DI;
	m_Instructions[Instruction::EI]  = &CPU::EI;
	m_Instructions[Instruction::EXT] = &CPU::ExecuteNextExtendedInstruction;
	m_Instructions[Instruction::BREAKPOINT_STOP] = &CPU::BREAKPOINT_STOP;
	m_Instructions[Instruction::BREAKPOINT_CONTINUE] = &CPU::BREAKPOINT_CONTINUE;

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

	m_Instructions[Instruction::LDI_A_aHL] = &CPU::Join<&CPU::LD_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL>, &CPU::UnaryInstruction_rr<Registers::RegisterIndexHL, &CPU::IncrementWord>>;
	m_Instructions[Instruction::LDD_A_aHL] = &CPU::Join<&CPU::LD_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL>, &CPU::UnaryInstruction_rr<Registers::RegisterIndexHL, &CPU::DecrementWord>>;
	m_Instructions[Instruction::LDI_aHL_A] = &CPU::Join<&CPU::LD_arr_r<Registers::RegisterIndexHL, Registers::RegisterIndexA>, &CPU::UnaryInstruction_rr<Registers::RegisterIndexHL, &CPU::IncrementWord>>;
	m_Instructions[Instruction::LDD_aHL_A] = &CPU::Join<&CPU::LD_arr_r<Registers::RegisterIndexHL, Registers::RegisterIndexA>, &CPU::UnaryInstruction_rr<Registers::RegisterIndexHL, &CPU::DecrementWord>>;

	// 16-bit load instructions
	m_Instructions[Instruction::LD_BC_nn]  = &CPU::LD_rr_nn<Registers::RegisterIndexBC>;
	m_Instructions[Instruction::LD_DE_nn]  = &CPU::LD_rr_nn<Registers::RegisterIndexDE>;
	m_Instructions[Instruction::LD_HL_nn]  = &CPU::LD_rr_nn<Registers::RegisterIndexHL>;
	m_Instructions[Instruction::LD_HL_SPn] = &CPU::LD_rr_rrn<Registers::RegisterIndexHL, Registers::RegisterIndexSP>;
	m_Instructions[Instruction::LD_SP_nn]  = &CPU::LD_rr_nn<Registers::RegisterIndexSP>;
	m_Instructions[Instruction::LD_SP_HL]  = &CPU::LD_rr_rr<Registers::RegisterIndexSP, Registers::RegisterIndexHL>;
	m_Instructions[Instruction::LD_ann_SP] = &CPU::LD_ann_rr<Registers::RegisterIndexSP>;

	// 8-bit add instructions
	m_Instructions[Instruction::ADD_A_A]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexA, &CPU::AddByte<false>>;
	m_Instructions[Instruction::ADD_A_B]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexB, &CPU::AddByte<false>>;
	m_Instructions[Instruction::ADD_A_C]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexC, &CPU::AddByte<false>>;
	m_Instructions[Instruction::ADD_A_D]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexD, &CPU::AddByte<false>>;
	m_Instructions[Instruction::ADD_A_E]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexE, &CPU::AddByte<false>>;
	m_Instructions[Instruction::ADD_A_H]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexH, &CPU::AddByte<false>>;
	m_Instructions[Instruction::ADD_A_L]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexL, &CPU::AddByte<false>>;
	m_Instructions[Instruction::ADD_A_n]   = &CPU::BinaryInstruction_r_n<Registers::RegisterIndexA, &CPU::AddByte<false>>;
	m_Instructions[Instruction::ADD_A_aHL] = &CPU::BinaryInstruction_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL, &CPU::AddByte<false>>;

	// 16-bit add instructions
	m_Instructions[Instruction::ADD_HL_BC] = &CPU::ADD_rr_rr<Registers::RegisterIndexHL, Registers::RegisterIndexBC>;
	m_Instructions[Instruction::ADD_HL_DE] = &CPU::ADD_rr_rr<Registers::RegisterIndexHL, Registers::RegisterIndexDE>;
	m_Instructions[Instruction::ADD_HL_HL] = &CPU::ADD_rr_rr<Registers::RegisterIndexHL, Registers::RegisterIndexHL>;
	m_Instructions[Instruction::ADD_HL_SP] = &CPU::ADD_rr_rr<Registers::RegisterIndexHL, Registers::RegisterIndexSP>;
	m_Instructions[Instruction::ADD_SP_n]  = &CPU::ADD_rr_n<Registers::RegisterIndexSP>;

	// 8-bit add + carry instructions
	m_Instructions[Instruction::ADC_A_A]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexA, &CPU::AddByte<true>>;
	m_Instructions[Instruction::ADC_A_B]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexB, &CPU::AddByte<true>>;
	m_Instructions[Instruction::ADC_A_C]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexC, &CPU::AddByte<true>>;
	m_Instructions[Instruction::ADC_A_D]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexD, &CPU::AddByte<true>>;
	m_Instructions[Instruction::ADC_A_E]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexE, &CPU::AddByte<true>>;
	m_Instructions[Instruction::ADC_A_H]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexH, &CPU::AddByte<true>>;
	m_Instructions[Instruction::ADC_A_L]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexL, &CPU::AddByte<true>>;
	m_Instructions[Instruction::ADC_A_n]   = &CPU::BinaryInstruction_r_n<Registers::RegisterIndexA, &CPU::AddByte<true>>;
	m_Instructions[Instruction::ADC_A_aHL] = &CPU::BinaryInstruction_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL, &CPU::AddByte<true>>;

	// 8-bit subtract instructions
	m_Instructions[Instruction::SUB_A_A]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexA, &CPU::SubtractByte<false>>;
	m_Instructions[Instruction::SUB_A_B]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexB, &CPU::SubtractByte<false>>;
	m_Instructions[Instruction::SUB_A_C]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexC, &CPU::SubtractByte<false>>;
	m_Instructions[Instruction::SUB_A_D]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexD, &CPU::SubtractByte<false>>;
	m_Instructions[Instruction::SUB_A_E]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexE, &CPU::SubtractByte<false>>;
	m_Instructions[Instruction::SUB_A_H]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexH, &CPU::SubtractByte<false>>;
	m_Instructions[Instruction::SUB_A_L]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexL, &CPU::SubtractByte<false>>;
	m_Instructions[Instruction::SUB_A_n]   = &CPU::BinaryInstruction_r_n<Registers::RegisterIndexA, &CPU::SubtractByte<false>>;
	m_Instructions[Instruction::SUB_A_aHL] = &CPU::BinaryInstruction_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL, &CPU::SubtractByte<false>>;

	// 8-bit subtract + carry instructions
	m_Instructions[Instruction::SBC_A_A]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexA, &CPU::SubtractByte<true>>;
	m_Instructions[Instruction::SBC_A_B]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexB, &CPU::SubtractByte<true>>;
	m_Instructions[Instruction::SBC_A_C]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexC, &CPU::SubtractByte<true>>;
	m_Instructions[Instruction::SBC_A_D]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexD, &CPU::SubtractByte<true>>;
	m_Instructions[Instruction::SBC_A_E]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexE, &CPU::SubtractByte<true>>;
	m_Instructions[Instruction::SBC_A_H]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexH, &CPU::SubtractByte<true>>;
	m_Instructions[Instruction::SBC_A_L]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexL, &CPU::SubtractByte<true>>;
	m_Instructions[Instruction::SBC_A_n]   = &CPU::BinaryInstruction_r_n<Registers::RegisterIndexA, &CPU::SubtractByte<true>>;
	m_Instructions[Instruction::SBC_A_aHL] = &CPU::BinaryInstruction_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL, &CPU::SubtractByte<true>>;

	// 8-bit AND instructions
	m_Instructions[Instruction::AND_A_A]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexA, &CPU::ANDByte>;
	m_Instructions[Instruction::AND_A_B]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexB, &CPU::ANDByte>;
	m_Instructions[Instruction::AND_A_C]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexC, &CPU::ANDByte>;
	m_Instructions[Instruction::AND_A_D]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexD, &CPU::ANDByte>;
	m_Instructions[Instruction::AND_A_E]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexE, &CPU::ANDByte>;
	m_Instructions[Instruction::AND_A_H]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexH, &CPU::ANDByte>;
	m_Instructions[Instruction::AND_A_L]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexL, &CPU::ANDByte>;
	m_Instructions[Instruction::AND_A_n]   = &CPU::BinaryInstruction_r_n<Registers::RegisterIndexA, &CPU::ANDByte>;
	m_Instructions[Instruction::AND_A_aHL] = &CPU::BinaryInstruction_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL, &CPU::ANDByte>;

	// 8-bit OR instructions
	m_Instructions[Instruction::OR_A_A]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexA, &CPU::ORByte>;
	m_Instructions[Instruction::OR_A_B]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexB, &CPU::ORByte>;
	m_Instructions[Instruction::OR_A_C]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexC, &CPU::ORByte>;
	m_Instructions[Instruction::OR_A_D]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexD, &CPU::ORByte>;
	m_Instructions[Instruction::OR_A_E]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexE, &CPU::ORByte>;
	m_Instructions[Instruction::OR_A_H]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexH, &CPU::ORByte>;
	m_Instructions[Instruction::OR_A_L]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexL, &CPU::ORByte>;
	m_Instructions[Instruction::OR_A_n]   = &CPU::BinaryInstruction_r_n<Registers::RegisterIndexA, &CPU::ORByte>;
	m_Instructions[Instruction::OR_A_aHL] = &CPU::BinaryInstruction_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL, &CPU::ORByte>;

	// 8-bit XOR instructions
	m_Instructions[Instruction::XOR_A_A]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexA, &CPU::XORByte>;
	m_Instructions[Instruction::XOR_A_B]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexB, &CPU::XORByte>;
	m_Instructions[Instruction::XOR_A_C]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexC, &CPU::XORByte>;
	m_Instructions[Instruction::XOR_A_D]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexD, &CPU::XORByte>;
	m_Instructions[Instruction::XOR_A_E]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexE, &CPU::XORByte>;
	m_Instructions[Instruction::XOR_A_H]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexH, &CPU::XORByte>;
	m_Instructions[Instruction::XOR_A_L]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexL, &CPU::XORByte>;
	m_Instructions[Instruction::XOR_A_n]   = &CPU::BinaryInstruction_r_n<Registers::RegisterIndexA, &CPU::XORByte>;
	m_Instructions[Instruction::XOR_A_aHL] = &CPU::BinaryInstruction_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL, &CPU::XORByte>;

	// 8-bit compare instructions
	m_Instructions[Instruction::CP_A_A]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexA, &CPU::SubtractByte<false>, false>;
	m_Instructions[Instruction::CP_A_B]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexB, &CPU::SubtractByte<false>, false>;
	m_Instructions[Instruction::CP_A_C]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexC, &CPU::SubtractByte<false>, false>;
	m_Instructions[Instruction::CP_A_D]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexD, &CPU::SubtractByte<false>, false>;
	m_Instructions[Instruction::CP_A_E]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexE, &CPU::SubtractByte<false>, false>;
	m_Instructions[Instruction::CP_A_H]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexH, &CPU::SubtractByte<false>, false>;
	m_Instructions[Instruction::CP_A_L]   = &CPU::BinaryInstruction_r_r<Registers::RegisterIndexA, Registers::RegisterIndexL, &CPU::SubtractByte<false>, false>;
	m_Instructions[Instruction::CP_A_n]   = &CPU::BinaryInstruction_r_n<Registers::RegisterIndexA, &CPU::SubtractByte<false>, false>;
	m_Instructions[Instruction::CP_A_aHL] = &CPU::BinaryInstruction_r_arr<Registers::RegisterIndexA, Registers::RegisterIndexHL, &CPU::SubtractByte<false>, false>;
	
	// 8-bit increment instructions
	m_Instructions[Instruction::INC_A]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexA, &CPU::IncrementByte>;
	m_Instructions[Instruction::INC_B]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexB, &CPU::IncrementByte>;
	m_Instructions[Instruction::INC_C]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexC, &CPU::IncrementByte>;
	m_Instructions[Instruction::INC_D]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexD, &CPU::IncrementByte>;
	m_Instructions[Instruction::INC_E]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexE, &CPU::IncrementByte>;
	m_Instructions[Instruction::INC_H]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexH, &CPU::IncrementByte>;
	m_Instructions[Instruction::INC_L]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexL, &CPU::IncrementByte>;
	m_Instructions[Instruction::INC_aHL] = &CPU::UnaryInstruction_arr<Registers::RegisterIndexHL, &CPU::IncrementByte>;

	// 16-bit increment instructions
	m_Instructions[Instruction::INC_BC] = &CPU::UnaryInstruction_rr<Registers::RegisterIndexBC, &CPU::IncrementWord>;
	m_Instructions[Instruction::INC_DE] = &CPU::UnaryInstruction_rr<Registers::RegisterIndexDE, &CPU::IncrementWord>;
	m_Instructions[Instruction::INC_HL] = &CPU::UnaryInstruction_rr<Registers::RegisterIndexHL, &CPU::IncrementWord>;
	m_Instructions[Instruction::INC_SP] = &CPU::UnaryInstruction_rr<Registers::RegisterIndexSP, &CPU::IncrementWord>;
	
	// 8-bit decrement instructions
	m_Instructions[Instruction::DEC_A]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexA, &CPU::DecrementByte>;
	m_Instructions[Instruction::DEC_B]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexB, &CPU::DecrementByte>;
	m_Instructions[Instruction::DEC_C]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexC, &CPU::DecrementByte>;
	m_Instructions[Instruction::DEC_D]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexD, &CPU::DecrementByte>;
	m_Instructions[Instruction::DEC_E]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexE, &CPU::DecrementByte>;
	m_Instructions[Instruction::DEC_H]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexH, &CPU::DecrementByte>;
	m_Instructions[Instruction::DEC_L]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexL, &CPU::DecrementByte>;
	m_Instructions[Instruction::DEC_aHL] = &CPU::UnaryInstruction_arr<Registers::RegisterIndexHL, &CPU::DecrementByte>;

	// 16-bit decrement instructions
	m_Instructions[Instruction::DEC_BC] = &CPU::UnaryInstruction_rr<Registers::RegisterIndexBC, &CPU::DecrementWord>;
	m_Instructions[Instruction::DEC_DE] = &CPU::UnaryInstruction_rr<Registers::RegisterIndexDE, &CPU::DecrementWord>;
	m_Instructions[Instruction::DEC_HL] = &CPU::UnaryInstruction_rr<Registers::RegisterIndexHL, &CPU::DecrementWord>;
	m_Instructions[Instruction::DEC_SP] = &CPU::UnaryInstruction_rr<Registers::RegisterIndexSP, &CPU::DecrementWord>;

	// 8-bit rotate instructions
	m_Instructions[Instruction::RLC_A] = &CPU::UnaryInstruction_r<Registers::RegisterIndexA, &CPU::RotateLeftByte>;
	m_Instructions[Instruction::RL_A]  = &CPU::UnaryInstruction_r<Registers::RegisterIndexA, &CPU::RotateLeftThroughCarryByte>;
	m_Instructions[Instruction::RRC_A] = &CPU::UnaryInstruction_r<Registers::RegisterIndexA, &CPU::RotateRightByte>;
	m_Instructions[Instruction::RR_A]  = &CPU::UnaryInstruction_r<Registers::RegisterIndexA, &CPU::RotateRightThroughCarryByte>;

	// Absolute jump instructions
	m_Instructions[Instruction::JP_nn]    = &CPU::JP_nn<Registers::FlagIndexInvalid, false>;
	m_Instructions[Instruction::JP_NZ_nn] = &CPU::JP_nn<Registers::FlagIndexZero, false>;
	m_Instructions[Instruction::JP_Z_nn]  = &CPU::JP_nn<Registers::FlagIndexZero, true>;
	m_Instructions[Instruction::JP_NC_nn] = &CPU::JP_nn<Registers::FlagIndexCarry, false>;
	m_Instructions[Instruction::JP_C_nn]  = &CPU::JP_nn<Registers::FlagIndexCarry, true>;
	m_Instructions[Instruction::JP_HL]    = &CPU::JP_rr<Registers::FlagIndexInvalid, false, Registers::RegisterIndexHL>;

	// Relative jump instructions
	m_Instructions[Instruction::JR_n]    = &CPU::JR_n<Registers::FlagIndexInvalid, false>;
	m_Instructions[Instruction::JR_NZ_n] = &CPU::JR_n<Registers::FlagIndexZero, false>;
	m_Instructions[Instruction::JR_Z_n]  = &CPU::JR_n<Registers::FlagIndexZero, true>;
	m_Instructions[Instruction::JR_NC_n] = &CPU::JR_n<Registers::FlagIndexCarry, false>;
	m_Instructions[Instruction::JR_C_n]  = &CPU::JR_n<Registers::FlagIndexCarry, true>;

	// Push instructions
	m_Instructions[Instruction::PUSH_AF] = &CPU::PUSH_rr<Registers::RegisterIndexAF>;
	m_Instructions[Instruction::PUSH_BC] = &CPU::PUSH_rr<Registers::RegisterIndexBC>;
	m_Instructions[Instruction::PUSH_DE] = &CPU::PUSH_rr<Registers::RegisterIndexDE>;
	m_Instructions[Instruction::PUSH_HL] = &CPU::PUSH_rr<Registers::RegisterIndexHL>;

	// Pop instructions
	m_Instructions[Instruction::POP_AF] = &CPU::POP_rr<Registers::RegisterIndexAF>;
	m_Instructions[Instruction::POP_BC] = &CPU::POP_rr<Registers::RegisterIndexBC>;
	m_Instructions[Instruction::POP_DE] = &CPU::POP_rr<Registers::RegisterIndexDE>;
	m_Instructions[Instruction::POP_HL] = &CPU::POP_rr<Registers::RegisterIndexHL>;

	// Call instructions
	m_Instructions[Instruction::CALL_nn]    = &CPU::CALL_nn<Registers::FlagIndexInvalid, false>;
	m_Instructions[Instruction::CALL_NZ_nn] = &CPU::CALL_nn<Registers::FlagIndexZero, false>;
	m_Instructions[Instruction::CALL_Z_nn]  = &CPU::CALL_nn<Registers::FlagIndexZero, true>;
	m_Instructions[Instruction::CALL_NC_nn] = &CPU::CALL_nn<Registers::FlagIndexCarry, false>;
	m_Instructions[Instruction::CALL_C_nn]  = &CPU::CALL_nn<Registers::FlagIndexCarry, true>;

	// Return instructions
	m_Instructions[Instruction::RET]    = &CPU::RET<Registers::FlagIndexInvalid, false>;
	m_Instructions[Instruction::RET_NZ] = &CPU::RET<Registers::FlagIndexZero, false>;
	m_Instructions[Instruction::RET_Z]  = &CPU::RET<Registers::FlagIndexZero, true>;
	m_Instructions[Instruction::RET_NC] = &CPU::RET<Registers::FlagIndexCarry, false>;
	m_Instructions[Instruction::RET_C]  = &CPU::RET<Registers::FlagIndexCarry, true>;
	m_Instructions[Instruction::RETI]   = &CPU::Join<&CPU::RET<Registers::FlagIndexInvalid, false>, &CPU::EI>;

	// Restart instructions
	m_Instructions[Instruction::RST_00] = &CPU::RST<0x00>;
	m_Instructions[Instruction::RST_08] = &CPU::RST<0x08>;
	m_Instructions[Instruction::RST_10] = &CPU::RST<0x10>;
	m_Instructions[Instruction::RST_18] = &CPU::RST<0x18>;
	m_Instructions[Instruction::RST_20] = &CPU::RST<0x20>;
	m_Instructions[Instruction::RST_28] = &CPU::RST<0x28>;
	m_Instructions[Instruction::RST_30] = &CPU::RST<0x30>;
	m_Instructions[Instruction::RST_38] = &CPU::RST<0x38>;

	// 8-bit rotate instructions
	m_ExtendedInstructions[ExtendedInstruction::RLC_A]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexA, &CPU::RotateLeftByte>;
	m_ExtendedInstructions[ExtendedInstruction::RLC_B]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexB, &CPU::RotateLeftByte>;
	m_ExtendedInstructions[ExtendedInstruction::RLC_C]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexC, &CPU::RotateLeftByte>;
	m_ExtendedInstructions[ExtendedInstruction::RLC_D]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexD, &CPU::RotateLeftByte>;
	m_ExtendedInstructions[ExtendedInstruction::RLC_E]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexE, &CPU::RotateLeftByte>;
	m_ExtendedInstructions[ExtendedInstruction::RLC_H]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexH, &CPU::RotateLeftByte>;
	m_ExtendedInstructions[ExtendedInstruction::RLC_L]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexL, &CPU::RotateLeftByte>;
	m_ExtendedInstructions[ExtendedInstruction::RLC_aHL] = &CPU::UnaryInstruction_arr<Registers::RegisterIndexHL, &CPU::RotateLeftByte>;

	m_ExtendedInstructions[ExtendedInstruction::RL_A]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexA, &CPU::RotateLeftThroughCarryByte>;
	m_ExtendedInstructions[ExtendedInstruction::RL_B]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexB, &CPU::RotateLeftThroughCarryByte>;
	m_ExtendedInstructions[ExtendedInstruction::RL_C]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexC, &CPU::RotateLeftThroughCarryByte>;
	m_ExtendedInstructions[ExtendedInstruction::RL_D]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexD, &CPU::RotateLeftThroughCarryByte>;
	m_ExtendedInstructions[ExtendedInstruction::RL_E]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexE, &CPU::RotateLeftThroughCarryByte>;
	m_ExtendedInstructions[ExtendedInstruction::RL_H]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexH, &CPU::RotateLeftThroughCarryByte>;
	m_ExtendedInstructions[ExtendedInstruction::RL_L]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexL, &CPU::RotateLeftThroughCarryByte>;
	m_ExtendedInstructions[ExtendedInstruction::RL_aHL] = &CPU::UnaryInstruction_arr<Registers::RegisterIndexHL, &CPU::RotateLeftThroughCarryByte>;

	m_ExtendedInstructions[ExtendedInstruction::RRC_A]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexA, &CPU::RotateRightByte>;
	m_ExtendedInstructions[ExtendedInstruction::RRC_B]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexB, &CPU::RotateRightByte>;
	m_ExtendedInstructions[ExtendedInstruction::RRC_C]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexC, &CPU::RotateRightByte>;
	m_ExtendedInstructions[ExtendedInstruction::RRC_D]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexD, &CPU::RotateRightByte>;
	m_ExtendedInstructions[ExtendedInstruction::RRC_E]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexE, &CPU::RotateRightByte>;
	m_ExtendedInstructions[ExtendedInstruction::RRC_H]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexH, &CPU::RotateRightByte>;
	m_ExtendedInstructions[ExtendedInstruction::RRC_L]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexL, &CPU::RotateRightByte>;
	m_ExtendedInstructions[ExtendedInstruction::RRC_aHL] = &CPU::UnaryInstruction_arr<Registers::RegisterIndexHL, &CPU::RotateRightByte>;

	m_ExtendedInstructions[ExtendedInstruction::RR_A]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexA, &CPU::RotateRightThroughCarryByte>;
	m_ExtendedInstructions[ExtendedInstruction::RR_B]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexB, &CPU::RotateRightThroughCarryByte>;
	m_ExtendedInstructions[ExtendedInstruction::RR_C]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexC, &CPU::RotateRightThroughCarryByte>;
	m_ExtendedInstructions[ExtendedInstruction::RR_D]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexD, &CPU::RotateRightThroughCarryByte>;
	m_ExtendedInstructions[ExtendedInstruction::RR_E]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexE, &CPU::RotateRightThroughCarryByte>;
	m_ExtendedInstructions[ExtendedInstruction::RR_H]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexH, &CPU::RotateRightThroughCarryByte>;
	m_ExtendedInstructions[ExtendedInstruction::RR_L]   = &CPU::UnaryInstruction_r<Registers::RegisterIndexL, &CPU::RotateRightThroughCarryByte>;
	m_ExtendedInstructions[ExtendedInstruction::RR_aHL] = &CPU::UnaryInstruction_arr<Registers::RegisterIndexHL, &CPU::RotateRightThroughCarryByte>;

	// 8-bit test bit instructions
	m_ExtendedInstructions[ExtendedInstruction::BIT_A_0] = &CPU::BIT_r_b<Registers::RegisterIndexA, 0>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_A_1] = &CPU::BIT_r_b<Registers::RegisterIndexA, 1>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_A_2] = &CPU::BIT_r_b<Registers::RegisterIndexA, 2>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_A_3] = &CPU::BIT_r_b<Registers::RegisterIndexA, 3>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_A_4] = &CPU::BIT_r_b<Registers::RegisterIndexA, 4>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_A_5] = &CPU::BIT_r_b<Registers::RegisterIndexA, 5>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_A_6] = &CPU::BIT_r_b<Registers::RegisterIndexA, 6>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_A_7] = &CPU::BIT_r_b<Registers::RegisterIndexA, 7>;

	m_ExtendedInstructions[ExtendedInstruction::BIT_B_0] = &CPU::BIT_r_b<Registers::RegisterIndexB, 0>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_B_1] = &CPU::BIT_r_b<Registers::RegisterIndexB, 1>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_B_2] = &CPU::BIT_r_b<Registers::RegisterIndexB, 2>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_B_3] = &CPU::BIT_r_b<Registers::RegisterIndexB, 3>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_B_4] = &CPU::BIT_r_b<Registers::RegisterIndexB, 4>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_B_5] = &CPU::BIT_r_b<Registers::RegisterIndexB, 5>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_B_6] = &CPU::BIT_r_b<Registers::RegisterIndexB, 6>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_B_7] = &CPU::BIT_r_b<Registers::RegisterIndexB, 7>;

	m_ExtendedInstructions[ExtendedInstruction::BIT_C_0] = &CPU::BIT_r_b<Registers::RegisterIndexC, 0>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_C_1] = &CPU::BIT_r_b<Registers::RegisterIndexC, 1>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_C_2] = &CPU::BIT_r_b<Registers::RegisterIndexC, 2>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_C_3] = &CPU::BIT_r_b<Registers::RegisterIndexC, 3>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_C_4] = &CPU::BIT_r_b<Registers::RegisterIndexC, 4>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_C_5] = &CPU::BIT_r_b<Registers::RegisterIndexC, 5>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_C_6] = &CPU::BIT_r_b<Registers::RegisterIndexC, 6>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_C_7] = &CPU::BIT_r_b<Registers::RegisterIndexC, 7>;

	m_ExtendedInstructions[ExtendedInstruction::BIT_D_0] = &CPU::BIT_r_b<Registers::RegisterIndexD, 0>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_D_1] = &CPU::BIT_r_b<Registers::RegisterIndexD, 1>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_D_2] = &CPU::BIT_r_b<Registers::RegisterIndexD, 2>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_D_3] = &CPU::BIT_r_b<Registers::RegisterIndexD, 3>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_D_4] = &CPU::BIT_r_b<Registers::RegisterIndexD, 4>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_D_5] = &CPU::BIT_r_b<Registers::RegisterIndexD, 5>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_D_6] = &CPU::BIT_r_b<Registers::RegisterIndexD, 6>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_D_7] = &CPU::BIT_r_b<Registers::RegisterIndexD, 7>;

	m_ExtendedInstructions[ExtendedInstruction::BIT_E_0] = &CPU::BIT_r_b<Registers::RegisterIndexE, 0>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_E_1] = &CPU::BIT_r_b<Registers::RegisterIndexE, 1>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_E_2] = &CPU::BIT_r_b<Registers::RegisterIndexE, 2>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_E_3] = &CPU::BIT_r_b<Registers::RegisterIndexE, 3>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_E_4] = &CPU::BIT_r_b<Registers::RegisterIndexE, 4>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_E_5] = &CPU::BIT_r_b<Registers::RegisterIndexE, 5>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_E_6] = &CPU::BIT_r_b<Registers::RegisterIndexE, 6>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_E_7] = &CPU::BIT_r_b<Registers::RegisterIndexE, 7>;

	m_ExtendedInstructions[ExtendedInstruction::BIT_H_0] = &CPU::BIT_r_b<Registers::RegisterIndexH, 0>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_H_1] = &CPU::BIT_r_b<Registers::RegisterIndexH, 1>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_H_2] = &CPU::BIT_r_b<Registers::RegisterIndexH, 2>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_H_3] = &CPU::BIT_r_b<Registers::RegisterIndexH, 3>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_H_4] = &CPU::BIT_r_b<Registers::RegisterIndexH, 4>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_H_5] = &CPU::BIT_r_b<Registers::RegisterIndexH, 5>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_H_6] = &CPU::BIT_r_b<Registers::RegisterIndexH, 6>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_H_7] = &CPU::BIT_r_b<Registers::RegisterIndexH, 7>;

	m_ExtendedInstructions[ExtendedInstruction::BIT_L_0] = &CPU::BIT_r_b<Registers::RegisterIndexL, 0>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_L_1] = &CPU::BIT_r_b<Registers::RegisterIndexL, 1>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_L_2] = &CPU::BIT_r_b<Registers::RegisterIndexL, 2>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_L_3] = &CPU::BIT_r_b<Registers::RegisterIndexL, 3>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_L_4] = &CPU::BIT_r_b<Registers::RegisterIndexL, 4>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_L_5] = &CPU::BIT_r_b<Registers::RegisterIndexL, 5>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_L_6] = &CPU::BIT_r_b<Registers::RegisterIndexL, 6>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_L_7] = &CPU::BIT_r_b<Registers::RegisterIndexL, 7>;

	m_ExtendedInstructions[ExtendedInstruction::BIT_aHL_0] = &CPU::BIT_arr_b<Registers::RegisterIndexHL, 0>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_aHL_1] = &CPU::BIT_arr_b<Registers::RegisterIndexHL, 1>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_aHL_2] = &CPU::BIT_arr_b<Registers::RegisterIndexHL, 2>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_aHL_3] = &CPU::BIT_arr_b<Registers::RegisterIndexHL, 3>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_aHL_4] = &CPU::BIT_arr_b<Registers::RegisterIndexHL, 4>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_aHL_5] = &CPU::BIT_arr_b<Registers::RegisterIndexHL, 5>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_aHL_6] = &CPU::BIT_arr_b<Registers::RegisterIndexHL, 6>;
	m_ExtendedInstructions[ExtendedInstruction::BIT_aHL_7] = &CPU::BIT_arr_b<Registers::RegisterIndexHL, 7>;
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
	instruction.m_ExtendedInstruction = instruction.m_Instruction == Instruction::EXT ? static_cast<ExtendedInstruction::Enum>(PeekNextByte()) : static_cast<ExtendedInstruction::Enum>(0);

	ExecuteInstruction(instruction.m_Instruction);

	return instruction;
}

bool CPU::HasBreakpoint(uint16_t a_Address) const
{
	return m_Memory.GetReplaced8(a_Address).has_value();
}

void CPU::SetBreakpoint(uint16_t a_Address, bool a_Enabled)
{
	if (a_Enabled)
	{
		m_Memory.Replace8(a_Address, Instruction::BREAKPOINT_STOP);
	}
	else
	{
		m_Memory.Restore8(a_Address);
	}
}

void CPU::SetBreakpointCallback(std::function<void()>&& a_Callback)
{
	m_BreakpointCallback = std::move(a_Callback);
}

void CPU::ExecuteInstruction(Instruction::Enum a_Instruction) noexcept
{
	(this->*(m_Instructions[a_Instruction]))();
	m_CycleCount += Instruction::GetCycles(a_Instruction).value_or(0);
}

void CPU::ExecuteNextExtendedInstruction() noexcept
{
	ExecuteExtendedInstruction(static_cast<ExtendedInstruction::Enum>(ReadNextByte()));
}

void CPU::ExecuteExtendedInstruction(ExtendedInstruction::Enum a_Instruction) noexcept
{
	(this->*(m_ExtendedInstructions[a_Instruction]))();
	m_CycleCount += ExtendedInstruction::GetCycles(a_Instruction).value_or(0);
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

template <bool Carry>
uint8_t CPU::AddByte(uint8_t a_Left, uint8_t a_Right) noexcept
{
	if constexpr (Carry)
	{
		if (m_Registers.GetCarry())
		{
			++a_Right;
		}
	}

	const uint16_t result = static_cast<uint16_t>(a_Left) + static_cast<uint16_t>(a_Right);

	m_Registers.SetZero((result & 0xFF) == 0);
	m_Registers.SetSubtract(false);
	m_Registers.SetHalfCarry((((a_Left & 0xF) + (a_Right & 0xF)) & 0xF0) != 0);
	m_Registers.SetCarry((result & 0xFF00) != 0);

	return static_cast<uint8_t>(result);
}

template <bool Carry>
uint8_t CPU::SubtractByte(uint8_t a_Left, uint8_t a_Right) noexcept
{
	if constexpr (Carry)
	{
		if (m_Registers.GetCarry())
		{
			++a_Right;
		}
	}

	const uint8_t value = AddByte<false>(a_Left, (~a_Right) + 1);

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

uint8_t CPU::SwapByte(uint8_t a_Value) noexcept
{
	const uint8_t result = ((a_Value & 0x0F) << 4) | ((a_Value & 0xF0) >> 4);

	m_Registers.SetZero(result == 0);
	m_Registers.SetSubtract(false);
	m_Registers.SetHalfCarry(false);
	m_Registers.SetCarry(false);

	return result;
}

uint8_t CPU::RotateLeftByte(uint8_t a_Value) noexcept
{
	const uint8_t result = (a_Value << 1) | (a_Value >> 7);

	m_Registers.SetZero(result == 0);
	m_Registers.SetSubtract(false);
	m_Registers.SetHalfCarry(false);
	m_Registers.SetCarry((a_Value & 0x80) != 0);

	return result;
}

uint8_t CPU::RotateLeftThroughCarryByte(uint8_t a_Value) noexcept
{
	const uint8_t result = (a_Value << 1) | (m_Registers.GetCarry() ? 0x01 : 0x00);

	m_Registers.SetZero(result == 0);
	m_Registers.SetSubtract(false);
	m_Registers.SetHalfCarry(false);
	m_Registers.SetCarry((a_Value & 0x80) != 0);

	return result;
}

uint8_t CPU::RotateRightByte(uint8_t a_Value) noexcept
{
	const uint8_t result = (a_Value >> 1) | (a_Value << 7);

	m_Registers.SetZero(result == 0);
	m_Registers.SetSubtract(false);
	m_Registers.SetHalfCarry(false);
	m_Registers.SetCarry((a_Value & 0x01) != 0);

	return result;
}

uint8_t CPU::RotateRightThroughCarryByte(uint8_t a_Value) noexcept
{
	const uint8_t result = (a_Value >> 1) | (m_Registers.GetCarry() ? 0x80 : 0x00);

	m_Registers.SetZero(result == 0);
	m_Registers.SetSubtract(false);
	m_Registers.SetHalfCarry(false);
	m_Registers.SetCarry((a_Value & 0x01) != 0);

	return result;
}

template <uint8_t Flag, bool Set>
bool CPU::Condition() noexcept
{
	if constexpr (Flag != Registers::FlagIndexInvalid)
	{
		return m_Registers.GetFlag(Flag) == Set;
	}
	else
	{
		return true;
	}
}

template <CPU::InstructionCallback Callback, CPU::InstructionCallback... Callbacks>
void CPU::Join() noexcept
{
	(this->*Callback)();

	if constexpr (sizeof...(Callbacks) > 0)
	{
		Join<Callbacks...>();
	}
}

template <uint8_t Destination, CPU::UnaryOperator8 Operator, bool Store>
void CPU::UnaryInstruction_r() noexcept
{
	const uint8_t destination_value = m_Registers.GetRegister8(Destination);
	const uint8_t result = (this->*Operator)(destination_value);
	if constexpr (Store)
	{
		m_Registers.SetRegister8(Destination, result);
	}
}

template <uint8_t Destination, CPU::UnaryOperator8 Operator, bool Store>
void CPU::UnaryInstruction_arr() noexcept
{
	const uint16_t address = m_Registers.GetRegister16(Destination);
	const uint8_t destination_value = m_Memory.Load8(address);
	const uint8_t result = (this->*Operator)(destination_value);
	if constexpr (Store)
	{
		m_Memory.Store8(address, result);
	}
}

template <uint8_t Destination, CPU::UnaryOperator16 Operator, bool Store>
void CPU::UnaryInstruction_rr() noexcept
{
	const uint16_t destination_value = m_Registers.GetRegister16(Destination);
	const uint16_t result = (this->*Operator)(destination_value);
	if constexpr (Store)
	{
		m_Registers.SetRegister16(Destination, result);
	}
}

template <uint8_t Destination, CPU::BinaryOperator8 Operator, bool Store>
void CPU::BinaryInstruction_r_x(uint8_t a_Value) noexcept
{
	const uint8_t destination_value = m_Registers.GetRegister8(Destination);
	const uint8_t result = (this->*Operator)(destination_value, a_Value);
	if constexpr (Store)
	{
		m_Registers.SetRegister8(Destination, result);
	}
}

template <uint8_t Destination, CPU::BinaryOperator8 Operator, bool Store>
void CPU::BinaryInstruction_r_n() noexcept
{
	const uint8_t value = ReadNextByte();
	BinaryInstruction_r_x<Destination, Operator, Store>(value);
}

template <uint8_t Destination, uint8_t Source, CPU::BinaryOperator8 Operator, bool Store>
void CPU::BinaryInstruction_r_r() noexcept
{
	const uint8_t value = m_Registers.GetRegister8(Source);
	BinaryInstruction_r_x<Destination, Operator, Store>(value);
}

template <uint8_t Destination, uint8_t Source, CPU::BinaryOperator8 Operator, bool Store>
void CPU::BinaryInstruction_r_arr() noexcept
{
	const uint16_t address = m_Registers.GetRegister16(Source);
	const uint8_t value = m_Memory.Load8(address);
	BinaryInstruction_r_x<Destination, Operator, Store>(value);
}

void CPU::NotImplemented() noexcept
{
	//assert(false && "This instruction has not been implemented.");
	if (m_Memory.Load8(m_Registers.GetPC() - 2) == Instruction::EXT)
	{
		m_Registers.SetPC(m_Registers.GetPC() - 2_u16);
	}
	else
	{
		m_Registers.SetPC(m_Registers.GetPC() - 1_u16);
	}

	if (m_BreakpointCallback)
	{
		m_BreakpointCallback();
	}
}

void CPU::NOP() noexcept
{
}

void CPU::DI() noexcept
{
	// TODO: implement interrupts
}

void CPU::EI() noexcept
{
	// TODO: implement interrupts
}

void CPU::BREAKPOINT_STOP() noexcept
{
	const uint16_t instruction_address = m_Registers.GetPC() - 1_u16;
	const auto replaced_byte = m_Memory.GetReplaced8(instruction_address);
	if (!replaced_byte)
	{
		NotImplemented();
		return;
	}

	m_Memory.Replace8(instruction_address, Instruction::BREAKPOINT_CONTINUE);
	m_Registers.SetPC(instruction_address);

	if (m_BreakpointCallback)
	{
		m_BreakpointCallback();
	}
}

void CPU::BREAKPOINT_CONTINUE() noexcept
{
	const uint16_t instruction_address = m_Registers.GetPC() - 1_u16;
	const auto replaced_byte = m_Memory.GetReplaced8(instruction_address);
	if (!replaced_byte)
	{
		NotImplemented();
		return;
	}

	m_Memory.Replace8(instruction_address, Instruction::BREAKPOINT_STOP);

	const auto instruction = static_cast<Instruction::Enum>(*replaced_byte);
	ExecuteInstruction(instruction);
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

template <uint8_t Flag, bool Set>
void CPU::JP_xx(uint16_t a_Address) noexcept
{
	if (Condition<Flag, Set>())
	{
		m_Registers.SetPC(a_Address);
	}
}

template <uint8_t Flag, bool Set>
void CPU::JP_nn() noexcept
{
	JP_xx<Flag, Set>(ReadNextWord());
}

template <uint8_t Flag, bool Set, uint8_t Source>
void CPU::JP_rr() noexcept
{
	JP_xx<Flag, Set>(m_Registers.GetRegister16(Source));
}

template <uint8_t Flag, bool Set>
void CPU::JR_x(uint8_t a_Offset) noexcept
{
	if ((a_Offset & 0x80) == 0)
	{
		const uint16_t address = m_Registers.GetPC() + a_Offset;
		JP_xx<Flag, Set>(address);
	}
	else
	{
		const uint16_t address = m_Registers.GetPC() - (static_cast<uint16_t>(static_cast<uint8_t>(~a_Offset)) + 1);
		JP_xx<Flag, Set>(address);
	}
}

template <uint8_t Flag, bool Set>
void CPU::JR_n() noexcept
{
	JR_x<Flag, Set>(ReadNextByte());
}

void CPU::PUSH_xx(uint16_t a_Value) noexcept
{
	const uint16_t address = m_Registers.GetSP() - 2_u16;
	m_Memory.Store16(address, a_Value);
	m_Registers.SetSP(address);
}

template <uint8_t Source>
void CPU::PUSH_rr() noexcept
{
	const uint16_t value = m_Registers.GetRegister16(Source);
	PUSH_xx(value);
}

uint16_t CPU::POP_xx() noexcept
{
	const uint16_t address = m_Registers.GetSP();
	const uint16_t value = m_Memory.Load16(address);
	m_Registers.SetSP(address + 2_u16);
	return value;
}

template <uint8_t Destination>
void CPU::POP_rr() noexcept
{
	const uint16_t value = POP_xx();
	m_Registers.SetRegister16(Destination, value);
}

template <uint8_t Flag, bool Set>
void CPU::CALL_xx(uint16_t a_Address) noexcept
{
	if (Condition<Flag, Set>())
	{
		PUSH_xx(m_Registers.GetPC());
		m_Registers.SetPC(a_Address);
	}
}

template <uint8_t Flag, bool Set>
void CPU::CALL_nn() noexcept
{
	const uint16_t address = ReadNextWord();
	CALL_xx<Flag, Set>(address);
}

template <uint8_t Flag, bool Set>
void CPU::RET() noexcept
{
	if (Condition<Flag, Set>())
	{
		const uint16_t address = POP_xx();
		m_Registers.SetPC(address);
	}
}

template <uint16_t Address>
void CPU::RST() noexcept
{
	CALL_xx<Registers::FlagIndexInvalid, false>(Address);
}

template <uint8_t Bit>
void CPU::BIT_x_b(uint8_t a_Value) noexcept
{
	m_Registers.SetZero((a_Value & (1 << Bit)) == 0);
	m_Registers.SetSubtract(false);
	m_Registers.SetHalfCarry(true);
}

template <uint8_t Source, uint8_t Bit>
void CPU::BIT_r_b() noexcept
{
	const uint8_t value = m_Registers.GetRegister8(Source);
	BIT_x_b<Bit>(value);
}

template <uint8_t Source, uint8_t Bit>
void CPU::BIT_arr_b() noexcept
{
	const uint16_t address = m_Registers.GetRegister16(Source);
	const uint8_t value = m_Memory.Load8(address);
	BIT_x_b<Bit>(value);
}