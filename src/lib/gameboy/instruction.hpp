#ifndef H_DEMU_GAMEBOY_INSTRUCTION
#define H_DEMU_GAMEBOY_INSTRUCTION

#include <gameboy/api.hpp>

#include <array>
#include <optional>

// Instruction naming convention:
// 0 parameters: <name>
// 1 parameter:  <name>_<param1>
// 2 parameters: <name>_<param1>_<param2>
//
// Parameter naming convention:
// If the parameter is a register, use the name of the register: A, B, HL, SP, etc.
// If the parameter is the next byte or word, use the letter 'n': n or nn.
// If the parameter is used as an address (like a pointer), prefix it with the letter 'a': aBC, aHL, ann, etc.

namespace Demu::Gameboy
{
	namespace Instruction
	{
		enum Enum : uint8_t
		{
			NOP  = 0x00,
			DI   = 0xF3,
			EI   = 0xFB,
			HALT = 0x76,
			STOP = 0x10,
			EXT  = 0xCB,

			// 8-bit load
			LD_A_A    = 0x7F,
			LD_A_B    = 0x78,
			LD_A_C    = 0x79,
			LD_A_D    = 0x7A,
			LD_A_E    = 0x7B,
			LD_A_H    = 0x7C,
			LD_A_L    = 0x7D,
			LD_A_n    = 0x3E,
			LD_A_aBC  = 0x0A,
			LD_A_aDE  = 0x1A,
			LD_A_aHL  = 0x7E,
			LD_A_ann  = 0xFA,
			LD_A_aFFC = 0xF2,
			LD_A_aFFn = 0xF0,
			
			LD_B_A    = 0x47,
			LD_B_B    = 0x40,
			LD_B_C    = 0x41,
			LD_B_D    = 0x42,
			LD_B_E    = 0x43,
			LD_B_H    = 0x44,
			LD_B_L    = 0x45,
			LD_B_n    = 0x06,
			LD_B_aHL  = 0x46,
			
			LD_C_A    = 0x4F,
			LD_C_B    = 0x48,
			LD_C_C    = 0x49,
			LD_C_D    = 0x4A,
			LD_C_E    = 0x4B,
			LD_C_H    = 0x4C,
			LD_C_L    = 0x4D,
			LD_C_n    = 0x0E,
			LD_C_aHL  = 0x4E,

			LD_D_A    = 0x57,
			LD_D_B    = 0x50,
			LD_D_C    = 0x51,
			LD_D_D    = 0x52,
			LD_D_E    = 0x53,
			LD_D_H    = 0x54,
			LD_D_L    = 0x55,
			LD_D_n    = 0x16,
			LD_D_aHL  = 0x56,
			
			LD_E_A    = 0x5F,
			LD_E_B    = 0x58,
			LD_E_C    = 0x59,
			LD_E_D    = 0x5A,
			LD_E_E    = 0x5B,
			LD_E_H    = 0x5C,
			LD_E_L    = 0x5D,
			LD_E_n    = 0x1E,
			LD_E_aHL  = 0x5E,
			
			LD_H_A    = 0x67,
			LD_H_B    = 0x60,
			LD_H_C    = 0x61,
			LD_H_D    = 0x62,
			LD_H_E    = 0x63,
			LD_H_H    = 0x64,
			LD_H_L    = 0x65,
			LD_H_n    = 0x26,
			LD_H_aHL  = 0x66,
			
			LD_L_A    = 0x6F,
			LD_L_B    = 0x68,
			LD_L_C    = 0x69,
			LD_L_D    = 0x6A,
			LD_L_E    = 0x6B,
			LD_L_H    = 0x6C,
			LD_L_L    = 0x6D,
			LD_L_n    = 0x2E,
			LD_L_aHL  = 0x6E,

			LD_aHL_A  = 0x77,
			LD_aHL_B  = 0x70,
			LD_aHL_C  = 0x71,
			LD_aHL_D  = 0x72,
			LD_aHL_E  = 0x73,
			LD_aHL_H  = 0x74,
			LD_aHL_L  = 0x75,
			LD_aHL_n  = 0x36,

			LD_aBC_A = 0x02,
			LD_aDE_A = 0x12,
			LD_ann_A = 0xEA,
			LD_aFFC_A = 0xE2,
			LD_aFFn_A = 0xE0,

			LDI_A_aHL = 0x2A,
			LDD_A_aHL = 0x3A,

			LDI_aHL_A = 0x22,
			LDD_aHL_A = 0x32,

			// 16-bit loads
			LD_BC_nn = 0x01,
			LD_DE_nn = 0x11,
			LD_HL_nn = 0x21,
			LD_HL_SPn = 0xF8,
			LD_SP_nn = 0x31,
			LD_SP_HL = 0xF9,
			LD_ann_SP = 0x08,

			PUSH_AF = 0xF5,
			PUSH_BC = 0xC5,
			PUSH_DE = 0xD5,
			PUSH_HL = 0xE5,
				
			POP_AF = 0xF1,
			POP_BC = 0xC1,
			POP_DE = 0xD1,
			POP_HL = 0xE1,

			// 8-bit add
			ADD_A_A  = 0x87,
			ADD_A_B  = 0x80,
			ADD_A_C  = 0x81,
			ADD_A_D  = 0x82,
			ADD_A_E  = 0x83,
			ADD_A_H  = 0x84,
			ADD_A_L  = 0x85,
			ADD_A_n  = 0xC6,
			ADD_A_aHL = 0x86,

			// 16-bit add
			ADD_HL_BC = 0x09,
			ADD_HL_DE = 0x19,
			ADD_HL_HL = 0x29,
			ADD_HL_SP = 0x39,
			ADD_SP_n  = 0xE8,

			// 8-bit add + carry
			ADC_A_A   = 0x8F,
			ADC_A_B   = 0x88,
			ADC_A_C   = 0x89,
			ADC_A_D   = 0x8A,
			ADC_A_E   = 0x8B,
			ADC_A_H   = 0x8C,
			ADC_A_L   = 0x8D,
			ADC_A_n   = 0xCE,
			ADC_A_aHL = 0x8E,

			// 8-bit sub
			SUB_A_A   = 0x97,
			SUB_A_B   = 0x90,
			SUB_A_C   = 0x91,
			SUB_A_D   = 0x92,
			SUB_A_E   = 0x93,
			SUB_A_H   = 0x94,
			SUB_A_L   = 0x95,
			SUB_A_n   = 0xD6,
			SUB_A_aHL = 0x96,

			// 8-bit sub + carry
			SBC_A_A   = 0x9F,
			SBC_A_B   = 0x98,
			SBC_A_C   = 0x99,
			SBC_A_D   = 0x9A,
			SBC_A_E   = 0x9B,
			SBC_A_H   = 0x9C,
			SBC_A_L   = 0x9D,
			SBC_A_n   = 0xDE,
			SBC_A_aHL = 0x9E,

			// 8-bit and
			AND_A_A   = 0xA7,
			AND_A_B   = 0xA0,
			AND_A_C   = 0xA1,
			AND_A_D   = 0xA2,
			AND_A_E   = 0xA3,
			AND_A_H   = 0xA4,
			AND_A_L   = 0xA5,
			AND_A_n   = 0xE6,
			AND_A_aHL = 0xA6,

			// 8-bit or
			OR_A_A   = 0xB7,
			OR_A_B   = 0xB0,
			OR_A_C   = 0xB1,
			OR_A_D   = 0xB2,
			OR_A_E   = 0xB3,
			OR_A_H   = 0xB4,
			OR_A_L   = 0xB5,
			OR_A_n   = 0xF6,
			OR_A_aHL = 0xB6,

			// 8-bit xor
			XOR_A_A   = 0xAF,
			XOR_A_B   = 0xA8,
			XOR_A_C   = 0xA9,
			XOR_A_D   = 0xAA,
			XOR_A_E   = 0xAB,
			XOR_A_H   = 0xAC,
			XOR_A_L   = 0xAD,
			XOR_A_n   = 0xEE,
			XOR_A_aHL = 0xAE,

			// 8-bit compare
			CP_A_A   = 0xBF,
			CP_A_B   = 0xB8,
			CP_A_C   = 0xB9,
			CP_A_D   = 0xBA,
			CP_A_E   = 0xBB,
			CP_A_H   = 0xBC,
			CP_A_L   = 0xBD,
			CP_A_n   = 0xFE,
			CP_A_aHL = 0xBE,

			// 8-bit increment
			INC_A   = 0x3C,
			INC_B   = 0x04,
			INC_C   = 0x0C,
			INC_D   = 0x14,
			INC_E   = 0x1C,
			INC_H   = 0x24,
			INC_L   = 0x2C,
			INC_aHL = 0x34,
			
			// 16-bit increment
			INC_BC = 0x03,
			INC_DE = 0x13,
			INC_HL = 0x23,
			INC_SP = 0x33,

			// 8-bit decrement
			DEC_A   = 0x3D,
			DEC_B   = 0x05,
			DEC_C   = 0x0D,
			DEC_D   = 0x15,
			DEC_E   = 0x1D,
			DEC_H   = 0x25,
			DEC_L   = 0x2D,
			DEC_aHL = 0x35,

			// 16-bit increment
			DEC_BC = 0x0B,
			DEC_DE = 0x1B,
			DEC_HL = 0x2B,
			DEC_SP = 0x3B,

			// 8-bit rotate
			RLC_A = 0x07,
			RL_A  = 0x17,
			RRC_A = 0x0F,
			RR_A  = 0x1F,

			// 8-bit decimal adjust
			DA_A  = 0x27,

			// 8-bit complement
			CPL_A = 0x2F,

			// Modify flags
			CCF   = 0x3F,
			SCF   = 0x37,

			// Absolute jump
			JP_nn    = 0xC3,
			JP_NZ_nn = 0xC2,
			JP_Z_nn  = 0xCA,
			JP_NC_nn = 0xD2,
			JP_C_nn  = 0xDA,
			JP_HL    = 0xE9,

			// Relative jump
			JR_n    = 0x18,
			JR_NZ_n = 0x20,
			JR_Z_n  = 0x28,
			JR_NC_n = 0x30,
			JR_C_n  = 0x38,

			// Call
			CALL_nn    = 0xCD,
			CALL_NZ_nn = 0xC4,
			CALL_Z_nn  = 0xCC,
			CALL_NC_nn = 0xD4,
			CALL_C_nn  = 0xDC,

			// Return
			RET    = 0xC9,
			RET_NZ = 0xC0,
			RET_Z  = 0xC8,
			RET_NC = 0xD0,
			RET_C  = 0xD8,
			RETI   = 0xD9,

			// Restart
			RST_00 = 0xC7,
			RST_08 = 0xCF,
			RST_10 = 0xD7,
			RST_18 = 0xDF,
			RST_20 = 0xE7,
			RST_28 = 0xEF,
			RST_30 = 0xF7,
			RST_38 = 0xFF,
		};

		namespace Internal
		{
			struct InstructionInfo
			{
				std::optional<uint16_t> m_Cycles;
				std::optional<uint16_t> m_Size;
				std::optional<std::string_view> m_Name;
			};

			constexpr std::array<InstructionInfo, 256> g_InstructionInfo = []() constexpr
			{
				std::array<InstructionInfo, 256> info;

				// 4 cycles, 1 byte
				info[Instruction::NOP]     = { 4_u16, 1_u16, "NOP" };
				info[Instruction::DI]      = { 4_u16, 1_u16, "DI" };
				info[Instruction::EI]      = { 4_u16, 1_u16, "EI" };
				info[Instruction::HALT]    = { 4_u16, 1_u16, "HALT" };
				info[Instruction::STOP]    = { 4_u16, 1_u16, "STOP" };
				info[Instruction::EXT]     = { 4_u16, 1_u16, "EXT" };
				info[Instruction::LD_A_A]  = { 4_u16, 1_u16, "LD A,A" };
				info[Instruction::LD_A_B]  = { 4_u16, 1_u16, "LD A,B" };
				info[Instruction::LD_A_C]  = { 4_u16, 1_u16, "LD A,C" };
				info[Instruction::LD_A_D]  = { 4_u16, 1_u16, "LD A,D" };
				info[Instruction::LD_A_E]  = { 4_u16, 1_u16, "LD A,E" };
				info[Instruction::LD_A_H]  = { 4_u16, 1_u16, "LD A,H" };
				info[Instruction::LD_A_L]  = { 4_u16, 1_u16, "LD A,L" };
				info[Instruction::LD_B_A]  = { 4_u16, 1_u16, "LD B,A" };
				info[Instruction::LD_B_B]  = { 4_u16, 1_u16, "LD B,B" };
				info[Instruction::LD_B_C]  = { 4_u16, 1_u16, "LD B,C" };
				info[Instruction::LD_B_D]  = { 4_u16, 1_u16, "LD B,D" };
				info[Instruction::LD_B_E]  = { 4_u16, 1_u16, "LD B,E" };
				info[Instruction::LD_B_H]  = { 4_u16, 1_u16, "LD B,H" };
				info[Instruction::LD_B_L]  = { 4_u16, 1_u16, "LD B,L" };
				info[Instruction::LD_C_A]  = { 4_u16, 1_u16, "LD C,A" };
				info[Instruction::LD_C_B]  = { 4_u16, 1_u16, "LD C,B" };
				info[Instruction::LD_C_C]  = { 4_u16, 1_u16, "LD C,C" };
				info[Instruction::LD_C_D]  = { 4_u16, 1_u16, "LD C,D" };
				info[Instruction::LD_C_E]  = { 4_u16, 1_u16, "LD C,E" };
				info[Instruction::LD_C_H]  = { 4_u16, 1_u16, "LD C,H" };
				info[Instruction::LD_C_L]  = { 4_u16, 1_u16, "LD C,L" };
				info[Instruction::LD_D_A]  = { 4_u16, 1_u16, "LD D,A" };
				info[Instruction::LD_D_B]  = { 4_u16, 1_u16, "LD D,B" };
				info[Instruction::LD_D_C]  = { 4_u16, 1_u16, "LD D,C" };
				info[Instruction::LD_D_D]  = { 4_u16, 1_u16, "LD D,D" };
				info[Instruction::LD_D_E]  = { 4_u16, 1_u16, "LD D,E" };
				info[Instruction::LD_D_H]  = { 4_u16, 1_u16, "LD D,H" };
				info[Instruction::LD_D_L]  = { 4_u16, 1_u16, "LD D,L" };
				info[Instruction::LD_E_A]  = { 4_u16, 1_u16, "LD E,A" };
				info[Instruction::LD_E_B]  = { 4_u16, 1_u16, "LD E,B" };
				info[Instruction::LD_E_C]  = { 4_u16, 1_u16, "LD E,C" };
				info[Instruction::LD_E_D]  = { 4_u16, 1_u16, "LD E,D" };
				info[Instruction::LD_E_E]  = { 4_u16, 1_u16, "LD E,E" };
				info[Instruction::LD_E_H]  = { 4_u16, 1_u16, "LD E,H" };
				info[Instruction::LD_E_L]  = { 4_u16, 1_u16, "LD E,L" };
				info[Instruction::LD_H_A]  = { 4_u16, 1_u16, "LD H,A" };
				info[Instruction::LD_H_B]  = { 4_u16, 1_u16, "LD H,B" };
				info[Instruction::LD_H_C]  = { 4_u16, 1_u16, "LD H,C" };
				info[Instruction::LD_H_D]  = { 4_u16, 1_u16, "LD H,D" };
				info[Instruction::LD_H_E]  = { 4_u16, 1_u16, "LD H,E" };
				info[Instruction::LD_H_H]  = { 4_u16, 1_u16, "LD H,H" };
				info[Instruction::LD_H_L]  = { 4_u16, 1_u16, "LD H,L" };
				info[Instruction::LD_L_A]  = { 4_u16, 1_u16, "LD L,A" };
				info[Instruction::LD_L_B]  = { 4_u16, 1_u16, "LD L,B" };
				info[Instruction::LD_L_C]  = { 4_u16, 1_u16, "LD L,C" };
				info[Instruction::LD_L_D]  = { 4_u16, 1_u16, "LD L,D" };
				info[Instruction::LD_L_E]  = { 4_u16, 1_u16, "LD L,E" };
				info[Instruction::LD_L_H]  = { 4_u16, 1_u16, "LD L,H" };
				info[Instruction::LD_L_L]  = { 4_u16, 1_u16, "LD L,L" };
				info[Instruction::ADD_A_A] = { 4_u16, 1_u16, "ADD A,A" };
				info[Instruction::ADD_A_B] = { 4_u16, 1_u16, "ADD A,B" };
				info[Instruction::ADD_A_C] = { 4_u16, 1_u16, "ADD A,C" };
				info[Instruction::ADD_A_D] = { 4_u16, 1_u16, "ADD A,D" };
				info[Instruction::ADD_A_E] = { 4_u16, 1_u16, "ADD A,E" };
				info[Instruction::ADD_A_H] = { 4_u16, 1_u16, "ADD A,H" };
				info[Instruction::ADD_A_L] = { 4_u16, 1_u16, "ADD A,L" };
				info[Instruction::ADC_A_A] = { 4_u16, 1_u16, "ADC A,A" };
				info[Instruction::ADC_A_B] = { 4_u16, 1_u16, "ADC A,B" };
				info[Instruction::ADC_A_C] = { 4_u16, 1_u16, "ADC A,C" };
				info[Instruction::ADC_A_D] = { 4_u16, 1_u16, "ADC A,D" };
				info[Instruction::ADC_A_E] = { 4_u16, 1_u16, "ADC A,E" };
				info[Instruction::ADC_A_H] = { 4_u16, 1_u16, "ADC A,H" };
				info[Instruction::ADC_A_L] = { 4_u16, 1_u16, "ADC A,L" };
				info[Instruction::SUB_A_A] = { 4_u16, 1_u16, "SUB A,A" };
				info[Instruction::SUB_A_B] = { 4_u16, 1_u16, "SUB A,B" };
				info[Instruction::SUB_A_C] = { 4_u16, 1_u16, "SUB A,C" };
				info[Instruction::SUB_A_D] = { 4_u16, 1_u16, "SUB A,D" };
				info[Instruction::SUB_A_E] = { 4_u16, 1_u16, "SUB A,E" };
				info[Instruction::SUB_A_H] = { 4_u16, 1_u16, "SUB A,H" };
				info[Instruction::SUB_A_L] = { 4_u16, 1_u16, "SUB A,L" };
				info[Instruction::SBC_A_A] = { 4_u16, 1_u16, "SBC A,A" };
				info[Instruction::SBC_A_B] = { 4_u16, 1_u16, "SBC A,B" };
				info[Instruction::SBC_A_C] = { 4_u16, 1_u16, "SBC A,C" };
				info[Instruction::SBC_A_D] = { 4_u16, 1_u16, "SBC A,D" };
				info[Instruction::SBC_A_E] = { 4_u16, 1_u16, "SBC A,E" };
				info[Instruction::SBC_A_H] = { 4_u16, 1_u16, "SBC A,H" };
				info[Instruction::SBC_A_L] = { 4_u16, 1_u16, "SBC A,L" };
				info[Instruction::AND_A_A] = { 4_u16, 1_u16, "AND A,A" };
				info[Instruction::AND_A_B] = { 4_u16, 1_u16, "AND A,B" };
				info[Instruction::AND_A_C] = { 4_u16, 1_u16, "AND A,C" };
				info[Instruction::AND_A_D] = { 4_u16, 1_u16, "AND A,D" };
				info[Instruction::AND_A_E] = { 4_u16, 1_u16, "AND A,E" };
				info[Instruction::AND_A_H] = { 4_u16, 1_u16, "AND A,H" };
				info[Instruction::AND_A_L] = { 4_u16, 1_u16, "AND A,L" };
				info[Instruction::OR_A_A]  = { 4_u16, 1_u16, "OR A,A" };
				info[Instruction::OR_A_B]  = { 4_u16, 1_u16, "OR A,B" };
				info[Instruction::OR_A_C]  = { 4_u16, 1_u16, "OR A,C" };
				info[Instruction::OR_A_D]  = { 4_u16, 1_u16, "OR A,D" };
				info[Instruction::OR_A_E]  = { 4_u16, 1_u16, "OR A,E" };
				info[Instruction::OR_A_H]  = { 4_u16, 1_u16, "OR A,H" };
				info[Instruction::OR_A_L]  = { 4_u16, 1_u16, "OR A,L" };
				info[Instruction::XOR_A_A] = { 4_u16, 1_u16, "XOR A,A" };
				info[Instruction::XOR_A_B] = { 4_u16, 1_u16, "XOR A,B" };
				info[Instruction::XOR_A_C] = { 4_u16, 1_u16, "XOR A,C" };
				info[Instruction::XOR_A_D] = { 4_u16, 1_u16, "XOR A,D" };
				info[Instruction::XOR_A_E] = { 4_u16, 1_u16, "XOR A,E" };
				info[Instruction::XOR_A_H] = { 4_u16, 1_u16, "XOR A,H" };
				info[Instruction::XOR_A_L] = { 4_u16, 1_u16, "XOR A,L" };
				info[Instruction::CP_A_A]  = { 4_u16, 1_u16, "CP A,A" };
				info[Instruction::CP_A_B]  = { 4_u16, 1_u16, "CP A,B" };
				info[Instruction::CP_A_C]  = { 4_u16, 1_u16, "CP A,C" };
				info[Instruction::CP_A_D]  = { 4_u16, 1_u16, "CP A,D" };
				info[Instruction::CP_A_E]  = { 4_u16, 1_u16, "CP A,E" };
				info[Instruction::CP_A_H]  = { 4_u16, 1_u16, "CP A,H" };
				info[Instruction::CP_A_L]  = { 4_u16, 1_u16, "CP A,L" };
				info[Instruction::INC_A]   = { 4_u16, 1_u16, "INC A" };
				info[Instruction::INC_B]   = { 4_u16, 1_u16, "INC B" };
				info[Instruction::INC_C]   = { 4_u16, 1_u16, "INC C" };
				info[Instruction::INC_D]   = { 4_u16, 1_u16, "INC D" };
				info[Instruction::INC_E]   = { 4_u16, 1_u16, "INC E" };
				info[Instruction::INC_H]   = { 4_u16, 1_u16, "INC H" };
				info[Instruction::INC_L]   = { 4_u16, 1_u16, "INC L" };
				info[Instruction::DEC_A]   = { 4_u16, 1_u16, "DEC A" };
				info[Instruction::DEC_B]   = { 4_u16, 1_u16, "DEC B" };
				info[Instruction::DEC_C]   = { 4_u16, 1_u16, "DEC C" };
				info[Instruction::DEC_D]   = { 4_u16, 1_u16, "DEC D" };
				info[Instruction::DEC_E]   = { 4_u16, 1_u16, "DEC E" };
				info[Instruction::DEC_H]   = { 4_u16, 1_u16, "DEC H" };
				info[Instruction::DEC_L]   = { 4_u16, 1_u16, "DEC L" };
				info[Instruction::DA_A]    = { 4_u16, 1_u16, "DA A" };
				info[Instruction::CPL_A]   = { 4_u16, 1_u16, "CPL A" };
				info[Instruction::CCF]     = { 4_u16, 1_u16, "CCF" };
				info[Instruction::SCF]     = { 4_u16, 1_u16, "SCF" };
				info[Instruction::JP_HL]   = { 4_u16, 1_u16, "JP HL" };
				info[Instruction::RLC_A]   = { 4_u16, 1_u16, "RLC A" };
				info[Instruction::RL_A]    = { 4_u16, 1_u16, "RL A" };
				info[Instruction::RRC_A]   = { 4_u16, 1_u16, "RRC A" };
				info[Instruction::RR_A]    = { 4_u16, 1_u16, "RR A" };

				// 8 cycles, 1 byte
				info[Instruction::LD_A_aBC]  = { 8_u16, 1_u16, "LD A,(BC)" };
				info[Instruction::LD_A_aDE]  = { 8_u16, 1_u16, "LD A,(DE)" };
				info[Instruction::LD_A_aHL]  = { 8_u16, 1_u16, "LD A,(HL)" };
				info[Instruction::LD_A_aFFC] = { 8_u16, 1_u16, "LD A,(FF00+C)" };
				info[Instruction::LD_B_aHL]  = { 8_u16, 1_u16, "LD B,(HL)" };
				info[Instruction::LD_C_aHL]  = { 8_u16, 1_u16, "LD C,(HL)" };
				info[Instruction::LD_D_aHL]  = { 8_u16, 1_u16, "LD D,(HL)" };
				info[Instruction::LD_E_aHL]  = { 8_u16, 1_u16, "LD E,(HL)" };
				info[Instruction::LD_H_aHL]  = { 8_u16, 1_u16, "LD H,(HL)" };
				info[Instruction::LD_L_aHL]  = { 8_u16, 1_u16, "LD L,(HL)" };
				info[Instruction::LD_aHL_A]  = { 8_u16, 1_u16, "LD (HL),A" };
				info[Instruction::LD_aHL_B]  = { 8_u16, 1_u16, "LD (HL),B" };
				info[Instruction::LD_aHL_C]  = { 8_u16, 1_u16, "LD (HL),C" };
				info[Instruction::LD_aHL_D]  = { 8_u16, 1_u16, "LD (HL),D" };
				info[Instruction::LD_aHL_E]  = { 8_u16, 1_u16, "LD (HL),E" };
				info[Instruction::LD_aHL_H]  = { 8_u16, 1_u16, "LD (HL),H" };
				info[Instruction::LD_aHL_L]  = { 8_u16, 1_u16, "LD (HL),L" };
				info[Instruction::LD_aBC_A]  = { 8_u16, 1_u16, "LD (BC),A" };
				info[Instruction::LD_aDE_A]  = { 8_u16, 1_u16, "LD (DE),A" };
				info[Instruction::LD_aFFC_A] = { 8_u16, 1_u16, "LD (FF00+C),A" };
				info[Instruction::LD_SP_HL]  = { 8_u16, 1_u16, "LD SP,HL" };
				info[Instruction::LDI_A_aHL] = { 8_u16, 1_u16, "LDI A,(HL)" };
				info[Instruction::LDD_A_aHL] = { 8_u16, 1_u16, "LDD A,(HL)" };
				info[Instruction::LDI_aHL_A] = { 8_u16, 1_u16, "LDI (HL),A" };
				info[Instruction::LDD_aHL_A] = { 8_u16, 1_u16, "LDD (HL),A" };
				info[Instruction::ADD_A_aHL] = { 8_u16, 1_u16, "ADD A,(HL)" };
				info[Instruction::ADD_HL_BC] = { 8_u16, 1_u16, "ADD HL,BC" };
				info[Instruction::ADD_HL_DE] = { 8_u16, 1_u16, "ADD HL,DE" };
				info[Instruction::ADD_HL_HL] = { 8_u16, 1_u16, "ADD HL,HL" };
				info[Instruction::ADD_HL_SP] = { 8_u16, 1_u16, "ADD HL,SP" };
				info[Instruction::ADC_A_aHL] = { 8_u16, 1_u16, "ADC A,(HL)" };
				info[Instruction::SUB_A_aHL] = { 8_u16, 1_u16, "SUB A,(HL)" };
				info[Instruction::SBC_A_aHL] = { 8_u16, 1_u16, "SBC A,(HL)" };
				info[Instruction::AND_A_aHL] = { 8_u16, 1_u16, "AND A,(HL)" };
				info[Instruction::OR_A_aHL]  = { 8_u16, 1_u16, "OR A,(HL)" };
				info[Instruction::XOR_A_aHL] = { 8_u16, 1_u16, "XOR A,(HL)" };
				info[Instruction::CP_A_aHL]  = { 8_u16, 1_u16, "CP A,(HL)" };
				info[Instruction::INC_BC]    = { 8_u16, 1_u16, "INC BC" };
				info[Instruction::INC_DE]    = { 8_u16, 1_u16, "INC DE" };
				info[Instruction::INC_HL]    = { 8_u16, 1_u16, "INC HL" };
				info[Instruction::INC_SP]    = { 8_u16, 1_u16, "INC SP" };
				info[Instruction::DEC_BC]    = { 8_u16, 1_u16, "DEC BC" };
				info[Instruction::DEC_DE]    = { 8_u16, 1_u16, "DEC DE" };
				info[Instruction::DEC_HL]    = { 8_u16, 1_u16, "DEC HL" };
				info[Instruction::DEC_SP]    = { 8_u16, 1_u16, "DEC SP" };
				info[Instruction::RET]       = { 8_u16, 1_u16, "RET" };
				info[Instruction::RET_NZ]    = { 8_u16, 1_u16, "RET NZ" };
				info[Instruction::RET_Z]     = { 8_u16, 1_u16, "RET Z" };
				info[Instruction::RET_NC]    = { 8_u16, 1_u16, "RET NC" };
				info[Instruction::RET_C]     = { 8_u16, 1_u16, "RET C" };
				info[Instruction::RETI]      = { 8_u16, 1_u16, "RETI" };

				// 8 cycles, 2 bytes
				info[Instruction::LD_A_n] =  { 8_u16, 2_u16, "LD A,n" };
				info[Instruction::LD_B_n] =  { 8_u16, 2_u16, "LD B,n" };
				info[Instruction::LD_C_n]  = { 8_u16, 2_u16, "LD C,n" };
				info[Instruction::LD_D_n]  = { 8_u16, 2_u16, "LD D,n" };
				info[Instruction::LD_E_n]  = { 8_u16, 2_u16, "LD E,n" };
				info[Instruction::LD_H_n]  = { 8_u16, 2_u16, "LD H,n" };
				info[Instruction::LD_L_n]  = { 8_u16, 2_u16, "LD L,n" };
				info[Instruction::ADD_A_n] = { 8_u16, 2_u16, "ADD A,n" };
				info[Instruction::ADC_A_n] = { 8_u16, 2_u16, "ADC A,n" };
				info[Instruction::SUB_A_n] = { 8_u16, 2_u16, "SUB A,n" };
				info[Instruction::SBC_A_n] = { 8_u16, 2_u16, "SBC A,n" };
				info[Instruction::AND_A_n] = { 8_u16, 2_u16, "ADD A,n" };
				info[Instruction::OR_A_n]  = { 8_u16, 2_u16, "OR A,n" };
				info[Instruction::XOR_A_n] = { 8_u16, 2_u16, "XOR A,n" };
				info[Instruction::CP_A_n]  = { 8_u16, 2_u16, "CP A,n" };
				info[Instruction::JR_n]    = { 8_u16, 2_u16, "JR n" };
				info[Instruction::JR_NZ_n] = { 8_u16, 2_u16, "JR NZ,n" };
				info[Instruction::JR_Z_n]  = { 8_u16, 2_u16, "JR Z,n" };
				info[Instruction::JR_NC_n] = { 8_u16, 2_u16, "JR NC,n" };
				info[Instruction::JR_C_n]  = { 8_u16, 2_u16, "JR C,n" };

				// 12 cycles, 1 byte
				info[Instruction::POP_AF] = { 12_u16, 1_u16, "POP AF" };
				info[Instruction::POP_BC] = { 12_u16, 1_u16, "POP BC" };
				info[Instruction::POP_DE] = { 12_u16, 1_u16, "POP DE" };
				info[Instruction::POP_HL] = { 12_u16, 1_u16, "POP HL" };

				// 12 cycles, 2 bytes
				info[Instruction::LD_A_aFFn] = { 12_u16, 2_u16, "LD A,(FF00+n)" };
				info[Instruction::LD_aHL_n]  = { 12_u16, 2_u16, "LD (HL),n" };
				info[Instruction::LD_aFFn_A] = { 12_u16, 2_u16, "LD (FF00+n),A" };
				info[Instruction::LD_HL_SPn] = { 12_u16, 2_u16, "LD HL,SP+n" };
				info[Instruction::INC_aHL]   = { 12_u16, 2_u16, "INC (HL)" };
				info[Instruction::DEC_aHL]   = { 12_u16, 2_u16, "DEC (HL)" };

				// 12 cycles, 3 bytes
				info[Instruction::LD_BC_nn]   = { 12_u16, 3_u16, "LD BC,nn" };
				info[Instruction::LD_DE_nn]   = { 12_u16, 3_u16, "LD DE,nn" };
				info[Instruction::LD_HL_nn]   = { 12_u16, 3_u16, "LD HL,nn" };
				info[Instruction::LD_SP_nn]   = { 12_u16, 3_u16, "LD SP,nn" };
				info[Instruction::JP_nn]      = { 12_u16, 3_u16, "JP nn" };
				info[Instruction::JP_NZ_nn]   = { 12_u16, 3_u16, "JP NZ nn" };
				info[Instruction::JP_Z_nn]    = { 12_u16, 3_u16, "JP Z nn" };
				info[Instruction::JP_NC_nn]   = { 12_u16, 3_u16, "JP NC nn" };
				info[Instruction::JP_C_nn]    = { 12_u16, 3_u16, "JP C nn" };
				info[Instruction::CALL_nn]    = { 12_u16, 3_u16, "CALL nn" };
				info[Instruction::CALL_NZ_nn] = { 12_u16, 3_u16, "CALL NZ nn" };
				info[Instruction::CALL_Z_nn]  = { 12_u16, 3_u16, "CALL Z nn" };
				info[Instruction::CALL_NC_nn] = { 12_u16, 3_u16, "CALL NC nn" };
				info[Instruction::CALL_C_nn]  = { 12_u16, 3_u16, "CALL C nn" };

				// 16 cycles, 1 byte
				info[Instruction::PUSH_AF] = { 16_u16, 1_u16, "PUSH AF" };
				info[Instruction::PUSH_BC] = { 16_u16, 1_u16, "PUSH BC" };
				info[Instruction::PUSH_DE] = { 16_u16, 1_u16, "PUSH DE" };
				info[Instruction::PUSH_HL] = { 16_u16, 1_u16, "PUSH HL" };

				// 16 cycles, 2 bytes
				info[Instruction::ADD_SP_n] = { 16_u16, 2_u16, "ADD SP,n" };

				// 16 cycles, 3 bytes
				info[Instruction::LD_A_ann] = { 16_u16, 3_u16, "LD A,(nn)" };
				info[Instruction::LD_ann_A] = { 16_u16, 3_u16, "LD (nn),A" };

				// 20 cycles, 3 bytes
				info[Instruction::LD_ann_SP] = { 20_u16, 3_u16, "LD (nn),SP" };

				// 32 cycles, 1 byte
				info[Instruction::RST_00] = { 32_u16, 1_u16, "RST 00" };
				info[Instruction::RST_08] = { 32_u16, 1_u16, "RST 08" };
				info[Instruction::RST_10] = { 32_u16, 1_u16, "RST 10" };
				info[Instruction::RST_18] = { 32_u16, 1_u16, "RST 18" };
				info[Instruction::RST_20] = { 32_u16, 1_u16, "RST 20" };
				info[Instruction::RST_28] = { 32_u16, 1_u16, "RST 28" };
				info[Instruction::RST_30] = { 32_u16, 1_u16, "RST 30" };
				info[Instruction::RST_38] = { 32_u16, 1_u16, "RST 38" };

				return info;
			}();
		}


		constexpr std::optional<uint16_t> GetCycles(Instruction::Enum a_Instruction)
		{
			return Internal::g_InstructionInfo[a_Instruction].m_Cycles;
		}

		constexpr std::optional<uint16_t> GetSize(Instruction::Enum a_Instruction)
		{
			return Internal::g_InstructionInfo[a_Instruction].m_Size;
		}

		constexpr std::optional<std::string_view> ToString(Instruction::Enum a_Instruction)
		{
			return Internal::g_InstructionInfo[a_Instruction].m_Name;
		}
	}
}

#endif