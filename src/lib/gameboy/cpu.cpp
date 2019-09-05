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
	InstructionBuilder<ExtendedOpcode::Enum> extended_instruction_builder;
	instruction_builder.Resize(256);
	extended_instruction_builder.Resize(256);

	// Misc instructions
	instruction_builder.Begin(Opcode::NOP);
	instruction_builder.Begin(Opcode::EXT, &CPU::DecodeExtendedInstruction);

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

	// 8-bit add instructions
	instruction_builder.Begin(Opcode::ADD_A_A, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexA, &CPU::AddByte<false>>);
	instruction_builder.Begin(Opcode::ADD_A_B, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexB, &CPU::AddByte<false>>);
	instruction_builder.Begin(Opcode::ADD_A_C, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexC, &CPU::AddByte<false>>);
	instruction_builder.Begin(Opcode::ADD_A_D, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexD, &CPU::AddByte<false>>);
	instruction_builder.Begin(Opcode::ADD_A_E, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexE, &CPU::AddByte<false>>);
	instruction_builder.Begin(Opcode::ADD_A_H, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexH, &CPU::AddByte<false>>);
	instruction_builder.Begin(Opcode::ADD_A_L, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexL, &CPU::AddByte<false>>);
	instruction_builder.Begin(Opcode::ADD_A_n).Cycle(&CPU::LD_c_n<0>, &CPU::BinaryOp_r_c<RegisterIndexA, 0, &CPU::AddByte<false>>);
	instruction_builder.Begin(Opcode::ADD_A_aHL).Cycle(&CPU::BinaryOp_r_arr<RegisterIndexA, RegisterIndexHL, &CPU::AddByte<false>>);

	// 16-bit add instructions
	instruction_builder.Begin(Opcode::ADD_HL_BC).Cycle(&CPU::ADD_rr_rr<Registers::RegisterIndexHL, Registers::RegisterIndexBC>);
	instruction_builder.Begin(Opcode::ADD_HL_DE).Cycle(&CPU::ADD_rr_rr<Registers::RegisterIndexHL, Registers::RegisterIndexDE>);
	instruction_builder.Begin(Opcode::ADD_HL_HL).Cycle(&CPU::ADD_rr_rr<Registers::RegisterIndexHL, Registers::RegisterIndexHL>);
	instruction_builder.Begin(Opcode::ADD_HL_SP).Cycle(&CPU::ADD_rr_rr<Registers::RegisterIndexHL, Registers::RegisterIndexSP>);
	instruction_builder.Begin(Opcode::ADD_SP_n).Cycle(&CPU::LD_c_n<0>).Cycle().Cycle(&CPU::ADD_rr_c<Registers::RegisterIndexSP,0>);

	// 8-bit subtract instructions
	instruction_builder.Begin(Opcode::SUB_A_A, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexA, &CPU::SubtractByte<false>>);
	instruction_builder.Begin(Opcode::SUB_A_B, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexB, &CPU::SubtractByte<false>>);
	instruction_builder.Begin(Opcode::SUB_A_C, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexC, &CPU::SubtractByte<false>>);
	instruction_builder.Begin(Opcode::SUB_A_D, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexD, &CPU::SubtractByte<false>>);
	instruction_builder.Begin(Opcode::SUB_A_E, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexE, &CPU::SubtractByte<false>>);
	instruction_builder.Begin(Opcode::SUB_A_H, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexH, &CPU::SubtractByte<false>>);
	instruction_builder.Begin(Opcode::SUB_A_L, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexL, &CPU::SubtractByte<false>>);
	instruction_builder.Begin(Opcode::SUB_A_n).Cycle(&CPU::LD_c_n<0>, &CPU::BinaryOp_r_c<RegisterIndexA, 0, &CPU::SubtractByte<false>>);
	instruction_builder.Begin(Opcode::SUB_A_aHL).Cycle(&CPU::BinaryOp_r_arr<RegisterIndexA, RegisterIndexHL, &CPU::SubtractByte<false>>);

	// 8-bit add + carry instructions
	instruction_builder.Begin(Opcode::ADC_A_A, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexA, &CPU::AddByte<true>>);
	instruction_builder.Begin(Opcode::ADC_A_B, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexB, &CPU::AddByte<true>>);
	instruction_builder.Begin(Opcode::ADC_A_C, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexC, &CPU::AddByte<true>>);
	instruction_builder.Begin(Opcode::ADC_A_D, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexD, &CPU::AddByte<true>>);
	instruction_builder.Begin(Opcode::ADC_A_E, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexE, &CPU::AddByte<true>>);
	instruction_builder.Begin(Opcode::ADC_A_H, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexH, &CPU::AddByte<true>>);
	instruction_builder.Begin(Opcode::ADC_A_L, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexL, &CPU::AddByte<true>>);
	instruction_builder.Begin(Opcode::ADC_A_n).Cycle(&CPU::LD_c_n<0>, &CPU::BinaryOp_r_c<RegisterIndexA, 0, &CPU::AddByte<true>>);
	instruction_builder.Begin(Opcode::ADC_A_aHL).Cycle(&CPU::BinaryOp_r_arr<RegisterIndexA, RegisterIndexHL, &CPU::AddByte<true>>);

	// 8-bit subtract + carry instructions
	instruction_builder.Begin(Opcode::SBC_A_A, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexA, &CPU::SubtractByte<true>>);
	instruction_builder.Begin(Opcode::SBC_A_B, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexB, &CPU::SubtractByte<true>>);
	instruction_builder.Begin(Opcode::SBC_A_C, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexC, &CPU::SubtractByte<true>>);
	instruction_builder.Begin(Opcode::SBC_A_D, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexD, &CPU::SubtractByte<true>>);
	instruction_builder.Begin(Opcode::SBC_A_E, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexE, &CPU::SubtractByte<true>>);
	instruction_builder.Begin(Opcode::SBC_A_H, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexH, &CPU::SubtractByte<true>>);
	instruction_builder.Begin(Opcode::SBC_A_L, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexL, &CPU::SubtractByte<true>>);
	instruction_builder.Begin(Opcode::SBC_A_n).Cycle(&CPU::LD_c_n<0>, &CPU::BinaryOp_r_c<RegisterIndexA, 0, &CPU::SubtractByte<true>>);
	instruction_builder.Begin(Opcode::SBC_A_aHL).Cycle(&CPU::BinaryOp_r_arr<RegisterIndexA, RegisterIndexHL, &CPU::SubtractByte<true>>);

	// 8-bit AND instructions
	instruction_builder.Begin(Opcode::AND_A_A, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexA, &CPU::ANDByte>);
	instruction_builder.Begin(Opcode::AND_A_B, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexB, &CPU::ANDByte>);
	instruction_builder.Begin(Opcode::AND_A_C, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexC, &CPU::ANDByte>);
	instruction_builder.Begin(Opcode::AND_A_D, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexD, &CPU::ANDByte>);
	instruction_builder.Begin(Opcode::AND_A_E, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexE, &CPU::ANDByte>);
	instruction_builder.Begin(Opcode::AND_A_H, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexH, &CPU::ANDByte>);
	instruction_builder.Begin(Opcode::AND_A_L, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexL, &CPU::ANDByte>);
	instruction_builder.Begin(Opcode::AND_A_n).Cycle(&CPU::LD_c_n<0>, &CPU::BinaryOp_r_c<RegisterIndexA, 0, &CPU::ANDByte>);
	instruction_builder.Begin(Opcode::AND_A_aHL).Cycle(&CPU::BinaryOp_r_arr<RegisterIndexA, RegisterIndexHL, &CPU::ANDByte>);

	// 8-bit OR instructions
	instruction_builder.Begin(Opcode::OR_A_A, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexA, &CPU::ORByte>);
	instruction_builder.Begin(Opcode::OR_A_B, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexB, &CPU::ORByte>);
	instruction_builder.Begin(Opcode::OR_A_C, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexC, &CPU::ORByte>);
	instruction_builder.Begin(Opcode::OR_A_D, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexD, &CPU::ORByte>);
	instruction_builder.Begin(Opcode::OR_A_E, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexE, &CPU::ORByte>);
	instruction_builder.Begin(Opcode::OR_A_H, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexH, &CPU::ORByte>);
	instruction_builder.Begin(Opcode::OR_A_L, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexL, &CPU::ORByte>);
	instruction_builder.Begin(Opcode::OR_A_n).Cycle(&CPU::LD_c_n<0>, &CPU::BinaryOp_r_c<RegisterIndexA, 0, &CPU::ORByte>);
	instruction_builder.Begin(Opcode::OR_A_aHL).Cycle(&CPU::BinaryOp_r_arr<RegisterIndexA, RegisterIndexHL, &CPU::ORByte>);

	// 8-bit XOR instructions
	instruction_builder.Begin(Opcode::XOR_A_A, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexA, &CPU::XORByte>);
	instruction_builder.Begin(Opcode::XOR_A_B, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexB, &CPU::XORByte>);
	instruction_builder.Begin(Opcode::XOR_A_C, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexC, &CPU::XORByte>);
	instruction_builder.Begin(Opcode::XOR_A_D, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexD, &CPU::XORByte>);
	instruction_builder.Begin(Opcode::XOR_A_E, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexE, &CPU::XORByte>);
	instruction_builder.Begin(Opcode::XOR_A_H, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexH, &CPU::XORByte>);
	instruction_builder.Begin(Opcode::XOR_A_L, &CPU::BinaryOp_r_r<RegisterIndexA, RegisterIndexL, &CPU::XORByte>);
	instruction_builder.Begin(Opcode::XOR_A_n).Cycle(&CPU::LD_c_n<0>, &CPU::BinaryOp_r_c<RegisterIndexA, 0, &CPU::XORByte>);
	instruction_builder.Begin(Opcode::XOR_A_aHL).Cycle(&CPU::BinaryOp_r_arr<RegisterIndexA, RegisterIndexHL, &CPU::XORByte>);

	// 8-bit compare instructions
	instruction_builder.Begin(Opcode::CP_A_A, &CPU::BinaryOp_r_r<Registers::RegisterIndexA, Registers::RegisterIndexA, &CPU::SubtractByte<false>, false>);
	instruction_builder.Begin(Opcode::CP_A_B, &CPU::BinaryOp_r_r<Registers::RegisterIndexA, Registers::RegisterIndexB, &CPU::SubtractByte<false>, false>);
	instruction_builder.Begin(Opcode::CP_A_C, &CPU::BinaryOp_r_r<Registers::RegisterIndexA, Registers::RegisterIndexC, &CPU::SubtractByte<false>, false>);
	instruction_builder.Begin(Opcode::CP_A_D, &CPU::BinaryOp_r_r<Registers::RegisterIndexA, Registers::RegisterIndexD, &CPU::SubtractByte<false>, false>);
	instruction_builder.Begin(Opcode::CP_A_E, &CPU::BinaryOp_r_r<Registers::RegisterIndexA, Registers::RegisterIndexE, &CPU::SubtractByte<false>, false>);
	instruction_builder.Begin(Opcode::CP_A_H, &CPU::BinaryOp_r_r<Registers::RegisterIndexA, Registers::RegisterIndexH, &CPU::SubtractByte<false>, false>);
	instruction_builder.Begin(Opcode::CP_A_L, &CPU::BinaryOp_r_r<Registers::RegisterIndexA, Registers::RegisterIndexL, &CPU::SubtractByte<false>, false>);
	instruction_builder.Begin(Opcode::CP_A_n).Cycle(&CPU::LD_c_n<0>, &CPU::BinaryOp_r_c<RegisterIndexA, 0, &CPU::SubtractByte<false>, false>);
	instruction_builder.Begin(Opcode::CP_A_aHL).Cycle(&CPU::BinaryOp_r_arr<RegisterIndexA, RegisterIndexHL, &CPU::SubtractByte<false>, false>);

	// 8-bit increment instructions
	instruction_builder.Begin(Opcode::INC_A, &CPU::UnaryOp_r<RegisterIndexA, &CPU::IncrementByte>);
	instruction_builder.Begin(Opcode::INC_B, &CPU::UnaryOp_r<RegisterIndexA, &CPU::IncrementByte>);
	instruction_builder.Begin(Opcode::INC_C, &CPU::UnaryOp_r<RegisterIndexA, &CPU::IncrementByte>);
	instruction_builder.Begin(Opcode::INC_D, &CPU::UnaryOp_r<RegisterIndexA, &CPU::IncrementByte>);
	instruction_builder.Begin(Opcode::INC_E, &CPU::UnaryOp_r<RegisterIndexA, &CPU::IncrementByte>);
	instruction_builder.Begin(Opcode::INC_H, &CPU::UnaryOp_r<RegisterIndexA, &CPU::IncrementByte>);
	instruction_builder.Begin(Opcode::INC_L, &CPU::UnaryOp_r<RegisterIndexA, &CPU::IncrementByte>);
	instruction_builder.Begin(Opcode::INC_aHL).Cycle(&CPU::LD_c_arr<0, RegisterIndexHL>, &CPU::UnaryOp_c<0, &CPU::IncrementByte>).Cycle(&CPU::LD_arr_c<RegisterIndexHL, 0>);

	// 16-bit increment instructions
	instruction_builder.Begin(Opcode::INC_BC).Cycle(&CPU::UnaryOp_rr<RegisterIndexBC, &CPU::IncrementWord>);
	instruction_builder.Begin(Opcode::INC_DE).Cycle(&CPU::UnaryOp_rr<RegisterIndexDE, &CPU::IncrementWord>);
	instruction_builder.Begin(Opcode::INC_HL).Cycle(&CPU::UnaryOp_rr<RegisterIndexHL, &CPU::IncrementWord>);
	instruction_builder.Begin(Opcode::INC_SP).Cycle(&CPU::UnaryOp_rr<RegisterIndexSP, &CPU::IncrementWord>);

	// 8-bit decrement instructions
	instruction_builder.Begin(Opcode::DEC_A, &CPU::UnaryOp_r<RegisterIndexA, &CPU::DecrementByte>);
	instruction_builder.Begin(Opcode::DEC_B, &CPU::UnaryOp_r<RegisterIndexA, &CPU::DecrementByte>);
	instruction_builder.Begin(Opcode::DEC_C, &CPU::UnaryOp_r<RegisterIndexA, &CPU::DecrementByte>);
	instruction_builder.Begin(Opcode::DEC_D, &CPU::UnaryOp_r<RegisterIndexA, &CPU::DecrementByte>);
	instruction_builder.Begin(Opcode::DEC_E, &CPU::UnaryOp_r<RegisterIndexA, &CPU::DecrementByte>);
	instruction_builder.Begin(Opcode::DEC_H, &CPU::UnaryOp_r<RegisterIndexA, &CPU::DecrementByte>);
	instruction_builder.Begin(Opcode::DEC_L, &CPU::UnaryOp_r<RegisterIndexA, &CPU::DecrementByte>);
	instruction_builder.Begin(Opcode::DEC_aHL).Cycle(&CPU::LD_c_arr<0, RegisterIndexHL>, &CPU::UnaryOp_c<0, &CPU::DecrementByte>).Cycle(&CPU::LD_arr_c<RegisterIndexHL, 0>);

	// 16-bit decrement instructions
	instruction_builder.Begin(Opcode::DEC_BC).Cycle(&CPU::UnaryOp_rr<RegisterIndexBC, &CPU::DecrementWord>);
	instruction_builder.Begin(Opcode::DEC_DE).Cycle(&CPU::UnaryOp_rr<RegisterIndexDE, &CPU::DecrementWord>);
	instruction_builder.Begin(Opcode::DEC_HL).Cycle(&CPU::UnaryOp_rr<RegisterIndexHL, &CPU::DecrementWord>);
	instruction_builder.Begin(Opcode::DEC_SP).Cycle(&CPU::UnaryOp_rr<RegisterIndexSP, &CPU::DecrementWord>);

	// Absolute jump instructions
	instruction_builder.Begin(Opcode::JP_nn).Cycle(&CPU::LD_c_n<1>).Cycle(&CPU::LD_c_n<0>).Cycle(&CPU::JP_cc);
	instruction_builder.Begin(Opcode::JP_NZ_nn).Cycle(&CPU::LD_c_n<1>).Cycle(&CPU::LD_c_n<0>, &CPU::FlagCondition<FlagIndexZero, false>).Cycle(&CPU::JP_cc);
	instruction_builder.Begin(Opcode::JP_Z_nn).Cycle(&CPU::LD_c_n<1>).Cycle(&CPU::LD_c_n<0>, &CPU::FlagCondition<FlagIndexZero, true>).Cycle(&CPU::JP_cc);
	instruction_builder.Begin(Opcode::JP_NC_nn).Cycle(&CPU::LD_c_n<1>).Cycle(&CPU::LD_c_n<0>, &CPU::FlagCondition<FlagIndexCarry, false>).Cycle(&CPU::JP_cc);
	instruction_builder.Begin(Opcode::JP_C_nn).Cycle(&CPU::LD_c_n<1>).Cycle(&CPU::LD_c_n<0>, &CPU::FlagCondition<FlagIndexCarry, true>).Cycle(&CPU::JP_cc);
	instruction_builder.Begin(Opcode::JP_HL, &CPU::JP_rr<RegisterIndexHL>);

	// Relative jump instructions
	instruction_builder.Begin(Opcode::JR_n).Cycle(&CPU::LD_c_n<0>).Cycle(&CPU::JR_c<0>);
	instruction_builder.Begin(Opcode::JR_NZ_n).Cycle(&CPU::LD_c_n<0>, &CPU::FlagCondition<FlagIndexZero, false>).Cycle(&CPU::JR_c<0>);
	instruction_builder.Begin(Opcode::JR_Z_n).Cycle(&CPU::LD_c_n<0>, &CPU::FlagCondition<FlagIndexZero, true>).Cycle(&CPU::JR_c<0>);
	instruction_builder.Begin(Opcode::JR_NC_n).Cycle(&CPU::LD_c_n<0>, &CPU::FlagCondition<FlagIndexCarry, false>).Cycle(&CPU::JR_c<0>);
	instruction_builder.Begin(Opcode::JR_C_n).Cycle(&CPU::LD_c_n<0>, &CPU::FlagCondition<FlagIndexCarry, true>).Cycle(&CPU::JR_c<0>);

	// Push instructions
	instruction_builder.Begin(Opcode::PUSH_AF, &CPU::LD_cc_rr<RegisterIndexAF>).Cycle(&CPU::LD_arr_c<RegisterIndexSP, 0>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::DecrementWord>).Cycle(&CPU::LD_arr_c<RegisterIndexSP, 1>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::DecrementWord>).Cycle();
	instruction_builder.Begin(Opcode::PUSH_BC, &CPU::LD_cc_rr<RegisterIndexAF>).Cycle(&CPU::LD_arr_c<RegisterIndexSP, 0>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::DecrementWord>).Cycle(&CPU::LD_arr_c<RegisterIndexSP, 1>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::DecrementWord>).Cycle();
	instruction_builder.Begin(Opcode::PUSH_DE, &CPU::LD_cc_rr<RegisterIndexAF>).Cycle(&CPU::LD_arr_c<RegisterIndexSP, 0>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::DecrementWord>).Cycle(&CPU::LD_arr_c<RegisterIndexSP, 1>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::DecrementWord>).Cycle();
	instruction_builder.Begin(Opcode::PUSH_HL, &CPU::LD_cc_rr<RegisterIndexAF>).Cycle(&CPU::LD_arr_c<RegisterIndexSP, 0>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::DecrementWord>).Cycle(&CPU::LD_arr_c<RegisterIndexSP, 1>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::DecrementWord>).Cycle();

	// Pop instructions
	instruction_builder.Begin(Opcode::POP_AF).Cycle(&CPU::LD_c_arr<1, RegisterIndexSP>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::IncrementWord>).Cycle(&CPU::LD_c_arr<1, RegisterIndexSP>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::IncrementWord>, &CPU::LD_rr_cc<RegisterIndexAF>);
	instruction_builder.Begin(Opcode::POP_BC).Cycle(&CPU::LD_c_arr<1, RegisterIndexSP>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::IncrementWord>).Cycle(&CPU::LD_c_arr<1, RegisterIndexSP>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::IncrementWord>, &CPU::LD_rr_cc<RegisterIndexBC>);
	instruction_builder.Begin(Opcode::POP_DE).Cycle(&CPU::LD_c_arr<1, RegisterIndexSP>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::IncrementWord>).Cycle(&CPU::LD_c_arr<1, RegisterIndexSP>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::IncrementWord>, &CPU::LD_rr_cc<RegisterIndexDE>);
	instruction_builder.Begin(Opcode::POP_HL).Cycle(&CPU::LD_c_arr<1, RegisterIndexSP>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::IncrementWord>).Cycle(&CPU::LD_c_arr<1, RegisterIndexSP>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::IncrementWord>, &CPU::LD_rr_cc<RegisterIndexHL>);

	// Call instructions
	instruction_builder.Begin(Opcode::CALL_nn, &CPU::LD_cc_rr<RegisterIndexPC>).Cycle(&CPU::LD_arr_c<RegisterIndexSP, 0>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::DecrementWord>).Cycle(&CPU::LD_arr_c<RegisterIndexSP, 1>, &CPU::UnaryOp_rr<RegisterIndexSP, &CPU::DecrementWord>).Cycle(&CPU::LD_c_n<1>).Cycle(&CPU::LD_c_n<0>).Cycle(&CPU::JP_cc);

	//instruction_builder.Begin(Opcode::CALL_nn] = &CPU::CALL_nn<Registers::FlagIndexInvalid, false>;
	//instruction_builder.Begin(Opcode::CALL_NZ_nn] = &CPU::CALL_nn<Registers::FlagIndexZero, false>;
	//instruction_builder.Begin(Opcode::CALL_Z_nn] = &CPU::CALL_nn<Registers::FlagIndexZero, true>;
	//instruction_builder.Begin(Opcode::CALL_NC_nn] = &CPU::CALL_nn<Registers::FlagIndexCarry, false>;
	//instruction_builder.Begin(Opcode::CALL_C_nn] = &CPU::CALL_nn<Registers::FlagIndexCarry, true>;
	//
	//// Return instructions
	//instruction_builder.Begin(Opcode::RET] = &CPU::RET<Registers::FlagIndexInvalid, false>;
	//instruction_builder.Begin(Opcode::RET_NZ] = &CPU::RET<Registers::FlagIndexZero, false>;
	//instruction_builder.Begin(Opcode::RET_Z] = &CPU::RET<Registers::FlagIndexZero, true>;
	//instruction_builder.Begin(Opcode::RET_NC] = &CPU::RET<Registers::FlagIndexCarry, false>;
	//instruction_builder.Begin(Opcode::RET_C] = &CPU::RET<Registers::FlagIndexCarry, true>;
	//instruction_builder.Begin(Opcode::RETI] = &CPU::Join<&CPU::RET<Registers::FlagIndexInvalid, false>, & CPU::EI>;
	//
	//// Restart instructions
	//instruction_builder.Begin(Opcode::RST_00] = &CPU::RST<0x00>;
	//instruction_builder.Begin(Opcode::RST_08] = &CPU::RST<0x08>;
	//instruction_builder.Begin(Opcode::RST_10] = &CPU::RST<0x10>;
	//instruction_builder.Begin(Opcode::RST_18] = &CPU::RST<0x18>;
	//instruction_builder.Begin(Opcode::RST_20] = &CPU::RST<0x20>;
	//instruction_builder.Begin(Opcode::RST_28] = &CPU::RST<0x28>;
	//instruction_builder.Begin(Opcode::RST_30] = &CPU::RST<0x30>;
	//instruction_builder.Begin(Opcode::RST_38] = &CPU::RST<0x38>;

	// 8-bit test bit instructions
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_A_0, &CPU::BIT_r_b<Registers::RegisterIndexA, 0>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_A_1, &CPU::BIT_r_b<Registers::RegisterIndexA, 1>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_A_2, &CPU::BIT_r_b<Registers::RegisterIndexA, 2>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_A_3, &CPU::BIT_r_b<Registers::RegisterIndexA, 3>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_A_4, &CPU::BIT_r_b<Registers::RegisterIndexA, 4>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_A_5, &CPU::BIT_r_b<Registers::RegisterIndexA, 5>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_A_6, &CPU::BIT_r_b<Registers::RegisterIndexA, 6>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_A_7, &CPU::BIT_r_b<Registers::RegisterIndexA, 7>);

	extended_instruction_builder.Begin(ExtendedOpcode::BIT_B_0, &CPU::BIT_r_b<Registers::RegisterIndexB, 0>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_B_1, &CPU::BIT_r_b<Registers::RegisterIndexB, 1>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_B_2, &CPU::BIT_r_b<Registers::RegisterIndexB, 2>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_B_3, &CPU::BIT_r_b<Registers::RegisterIndexB, 3>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_B_4, &CPU::BIT_r_b<Registers::RegisterIndexB, 4>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_B_5, &CPU::BIT_r_b<Registers::RegisterIndexB, 5>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_B_6, &CPU::BIT_r_b<Registers::RegisterIndexB, 6>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_B_7, &CPU::BIT_r_b<Registers::RegisterIndexB, 7>);

	extended_instruction_builder.Begin(ExtendedOpcode::BIT_C_0, &CPU::BIT_r_b<Registers::RegisterIndexC, 0>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_C_1, &CPU::BIT_r_b<Registers::RegisterIndexC, 1>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_C_2, &CPU::BIT_r_b<Registers::RegisterIndexC, 2>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_C_3, &CPU::BIT_r_b<Registers::RegisterIndexC, 3>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_C_4, &CPU::BIT_r_b<Registers::RegisterIndexC, 4>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_C_5, &CPU::BIT_r_b<Registers::RegisterIndexC, 5>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_C_6, &CPU::BIT_r_b<Registers::RegisterIndexC, 6>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_C_7, &CPU::BIT_r_b<Registers::RegisterIndexC, 7>);

	extended_instruction_builder.Begin(ExtendedOpcode::BIT_D_0, &CPU::BIT_r_b<Registers::RegisterIndexD, 0>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_D_1, &CPU::BIT_r_b<Registers::RegisterIndexD, 1>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_D_2, &CPU::BIT_r_b<Registers::RegisterIndexD, 2>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_D_3, &CPU::BIT_r_b<Registers::RegisterIndexD, 3>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_D_4, &CPU::BIT_r_b<Registers::RegisterIndexD, 4>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_D_5, &CPU::BIT_r_b<Registers::RegisterIndexD, 5>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_D_6, &CPU::BIT_r_b<Registers::RegisterIndexD, 6>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_D_7, &CPU::BIT_r_b<Registers::RegisterIndexD, 7>);

	extended_instruction_builder.Begin(ExtendedOpcode::BIT_E_0, &CPU::BIT_r_b<Registers::RegisterIndexE, 0>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_E_1, &CPU::BIT_r_b<Registers::RegisterIndexE, 1>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_E_2, &CPU::BIT_r_b<Registers::RegisterIndexE, 2>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_E_3, &CPU::BIT_r_b<Registers::RegisterIndexE, 3>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_E_4, &CPU::BIT_r_b<Registers::RegisterIndexE, 4>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_E_5, &CPU::BIT_r_b<Registers::RegisterIndexE, 5>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_E_6, &CPU::BIT_r_b<Registers::RegisterIndexE, 6>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_E_7, &CPU::BIT_r_b<Registers::RegisterIndexE, 7>);

	extended_instruction_builder.Begin(ExtendedOpcode::BIT_H_0, &CPU::BIT_r_b<Registers::RegisterIndexH, 0>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_H_1, &CPU::BIT_r_b<Registers::RegisterIndexH, 1>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_H_2, &CPU::BIT_r_b<Registers::RegisterIndexH, 2>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_H_3, &CPU::BIT_r_b<Registers::RegisterIndexH, 3>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_H_4, &CPU::BIT_r_b<Registers::RegisterIndexH, 4>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_H_5, &CPU::BIT_r_b<Registers::RegisterIndexH, 5>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_H_6, &CPU::BIT_r_b<Registers::RegisterIndexH, 6>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_H_7, &CPU::BIT_r_b<Registers::RegisterIndexH, 7>);

	extended_instruction_builder.Begin(ExtendedOpcode::BIT_L_0, &CPU::BIT_r_b<Registers::RegisterIndexL, 0>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_L_1, &CPU::BIT_r_b<Registers::RegisterIndexL, 1>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_L_2, &CPU::BIT_r_b<Registers::RegisterIndexL, 2>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_L_3, &CPU::BIT_r_b<Registers::RegisterIndexL, 3>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_L_4, &CPU::BIT_r_b<Registers::RegisterIndexL, 4>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_L_5, &CPU::BIT_r_b<Registers::RegisterIndexL, 5>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_L_6, &CPU::BIT_r_b<Registers::RegisterIndexL, 6>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_L_7, &CPU::BIT_r_b<Registers::RegisterIndexL, 7>);

	extended_instruction_builder.Begin(ExtendedOpcode::BIT_aHL_0).Cycle(&CPU::BIT_arr_b<Registers::RegisterIndexHL, 0>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_aHL_1).Cycle(&CPU::BIT_arr_b<Registers::RegisterIndexHL, 1>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_aHL_2).Cycle(&CPU::BIT_arr_b<Registers::RegisterIndexHL, 2>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_aHL_3).Cycle(&CPU::BIT_arr_b<Registers::RegisterIndexHL, 3>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_aHL_4).Cycle(&CPU::BIT_arr_b<Registers::RegisterIndexHL, 4>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_aHL_5).Cycle(&CPU::BIT_arr_b<Registers::RegisterIndexHL, 5>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_aHL_6).Cycle(&CPU::BIT_arr_b<Registers::RegisterIndexHL, 6>);
	extended_instruction_builder.Begin(ExtendedOpcode::BIT_aHL_7).Cycle(&CPU::BIT_arr_b<Registers::RegisterIndexHL, 7>);

	// Build instruction sets
	m_Instructions = instruction_builder.Build();
	m_ExtendedInstructions = extended_instruction_builder.Build();

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
	return m_OpFront == m_OpDone;
}

void CPU::Reset()
{
	m_OpBack = m_OpFront;

	#ifdef _DEBUG
	std::memset(m_MicroOps, 0, sizeof(m_MicroOps));
	#endif

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
	#ifdef _DEBUG
	assert(m_MicroOps[m_OpBack] == nullptr);
	#endif

	m_MicroOps[m_OpBack] = a_Op;
	++m_OpBack;
	m_OpBack %= std::size(m_MicroOps);
}

CPU::MicroOp CPU::PopOp()
{
	const MicroOp op = m_MicroOps[m_OpFront];

	#ifdef _DEBUG
	assert(m_MicroOps[m_OpFront] != nullptr);
	m_MicroOps[m_OpFront] = nullptr;
	#endif

	++m_OpFront;
	m_OpFront %= std::size(m_MicroOps);

	return op;
}

void CPU::InsertOP(MicroOp a_Op, size_t a_Index)
{
	for (size_t i = m_OpBack; i != a_Index; i = (i - 1) % std::size(m_MicroOps))
	{
		const size_t prev = (i - 1) % std::size(m_MicroOps);

		m_MicroOps[i] = m_MicroOps[prev];
	}

	m_MicroOps[a_Index] = a_Op;
	++m_OpBack;
	m_OpBack %= std::size(m_MicroOps);
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
void CPU::UnaryOp_c() noexcept
{
	const uint8_t destination_value = m_OpCache.GetByte<Destination>();
	const uint8_t result = (this->*Op)(destination_value);
	if constexpr (Store)
	{
		m_OpCache.SetByte<Destination>(result);
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
template <uint8_t Destination, uint8_t Source, CPU::BinaryOp8 Op, bool Store>
void CPU::BinaryOp_r_r() noexcept
{
	const uint8_t value = GetRegister8(Source);
	BinaryOp_r_x<Destination, Op, Store>(value);
}

template <uint8_t Destination, uint8_t Source, CPU::BinaryOp8 Op, bool Store>
void CPU::BinaryOp_r_c() noexcept
{
	const uint8_t value = m_OpCache.GetByte<Source>();
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
	const auto opcode = static_cast<Opcode::Enum>(ReadNextByte());
	const size_t instruction_size = m_Instructions->GetInstructionSize(opcode);
	const MicroOp* const ops = m_Instructions->GetInstructionOps(opcode);

	for (size_t i = 0; i < instruction_size; ++i)
	{
		PushOp(ops[i]);
	}

	m_OpDone = m_OpBack;

	PushOp(&CPU::DecodeInstruction);
}

void CPU::DecodeExtendedInstruction()
{
	const auto opcode = static_cast<ExtendedOpcode::Enum>(ReadNextByte());
	const size_t instruction_size = m_ExtendedInstructions->GetInstructionSize(opcode);
	const MicroOp* const ops = m_ExtendedInstructions->GetInstructionOps(opcode);

	for (size_t i = 0; i < instruction_size; ++i)
	{
		InsertOP(ops[i], m_OpDone);
		++m_OpDone;
		m_OpDone %= std::size(m_MicroOps);
	}
}

void CPU::Break()
{
	m_OpBreak = true;
}

void CPU::Skip()
{
	while (m_OpFront != m_OpDone)
	{
		PopOp();
	}
}

template <uint8_t Flag, bool Set>
void CPU::FlagCondition()
{
	if (GetFlag(Flag) != Set)
	{
		Skip();
	}
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

template <uint8_t Destination>
void CPU::ADD_rr_xx(uint16_t a_Value) noexcept
{
	SetRegister16(Destination, a_Value);
}

template <uint8_t Destination, uint8_t Source>
void CPU::ADD_rr_c() noexcept
{
	const uint8_t value = m_OpCache.GetByte<Source>();
	ADD_rr_xx<Destination>(value);
}

template <uint8_t Destination, uint8_t Source>
void CPU::ADD_rr_rr() noexcept
{
	const uint16_t value = GetRegister16(Source);
	ADD_rr_xx<Destination>(value);
}

void CPU::JP_xx(uint16_t a_Address) noexcept
{
	SetPC(a_Address);
}

void CPU::JP_cc() noexcept
{
	JP_xx(m_OpCache.GetWord());
}

template <uint8_t Source>
void CPU::JP_rr() noexcept
{
	const uint16_t address = GetRegister16(Source);
	JP_xx(address);
}

void CPU::JR_x(uint8_t a_Offset) noexcept
{
	uint16_t address;
	if ((a_Offset & 0x80) == 0)
	{
		address = GetPC() + a_Offset;
	}
	else
	{
		address = GetPC() - (static_cast<uint16_t>(static_cast<uint8_t>(~a_Offset)) + 1);
	}

	JP_xx(address);
}

template <uint8_t Source>
void CPU::JR_c() noexcept
{
	const uint8_t offset = m_OpCache.GetByte<Source>();
	JR_x(offset);
}

void CPU::PUSH_xx(uint16_t a_Value) noexcept
{
	const uint16_t address = GetSP() - 2_u16;
	m_Memory.Store16(address, a_Value);
	SetSP(address);
}

template <uint8_t Source>
void CPU::PUSH_rr() noexcept
{
	const uint16_t value = GetRegister16(Source);
	PUSH_xx(value);
}

uint16_t CPU::POP_xx() noexcept
{
	const uint16_t address = GetSP();
	const uint16_t value = m_Memory.Load16(address);
	SetSP(address + 2_u16);
	return value;
}

template <uint8_t Destination>
void CPU::POP_rr() noexcept
{
	const uint16_t value = POP_xx();
	SetRegister16(Destination, value);
}

void CPU::CALL_xx(uint16_t a_Address) noexcept
{
	PUSH_xx(GetPC());
	SetPC(a_Address);
}

void CPU::CALL_nn() noexcept
{
	const uint16_t address = ReadNextWord();
	CALL_xx(address);
}

template <uint8_t Flag, bool Set>
void CPU::RET() noexcept
{
	if (Condition<Flag, Set>())
	{
		const uint16_t address = POP_xx();
		SetPC(address);
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
	SetZero((a_Value & (1 << Bit)) == 0);
	SetSubtract(false);
	SetHalfCarry(true);
}

template <uint8_t Source, uint8_t Bit>
void CPU::BIT_r_b() noexcept
{
	const uint8_t value = GetRegister8(Source);
	BIT_x_b<Bit>(value);
}

template <uint8_t Source, uint8_t Bit>
void CPU::BIT_arr_b() noexcept
{
	const uint16_t address = GetRegister16(Source);
	const uint8_t value = m_Memory.Load8(address);
	BIT_x_b<Bit>(value);
}