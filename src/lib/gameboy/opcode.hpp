#ifndef H_AMBER_GAMEBOY_OPCODE
#define H_AMBER_GAMEBOY_OPCODE

#include <gameboy/api.hpp>

#include <array>
#include <optional>

// Opcode naming convention:
// 0 parameters: <name>
// 1 parameter:  <name>_<param1>
// 2 parameters: <name>_<param1>_<param2>
//
// Parameter naming convention:
// If the parameter is a register, use the name of the register: A, B, HL, SP, etc.
// If the parameter is the next byte or word, use the letter 'n': n or nn.
// If the parameter is used as an address (like a pointer), prefix it with the letter 'a': aBC, aHL, ann, etc.

namespace Amber::Gameboy
{
	namespace Opcode
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

			// Push
			PUSH_AF = 0xF5,
			PUSH_BC = 0xC5,
			PUSH_DE = 0xD5,
			PUSH_HL = 0xE5,

			// Pop
			POP_AF = 0xF1,
			POP_BC = 0xC1,
			POP_DE = 0xD1,
			POP_HL = 0xE1,

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

			// The opcodes below do not exist on the gameboy and can instead be used by the debugger

			// Software interrupts
			BREAKPOINT_STOP     = 0xFC,
			BREAKPOINT_CONTINUE = 0xFD,

			// Reserved
			RESERVED_FC = 0xD3,
			RESERVED_DB = 0xDB,
			RESERVED_DD = 0xDD,
			RESERVED_E3 = 0xE3,
			RESERVED_E4 = 0xE4,
			RESERVED_EB = 0xEB,
			RESERVED_EC = 0xEC,
			RESERVED_ED = 0xED,
			RESERVED_F4 = 0xF4,
			RESERVED_F5 = 0xF5,
		};

		namespace Internal
		{
			struct OpcodeInfo
			{
				std::optional<uint16_t> m_Cycles;
				std::optional<uint16_t> m_Size;
				std::optional<std::string_view> m_Name;
			};

			constexpr std::array<OpcodeInfo, 256> g_OpcodeInfo = []() constexpr
			{
				std::array<OpcodeInfo, 256> info;

				// 4 cycles, 1 byte
				info[Opcode::NOP]     = { 4_u16, 1_u16, "NOP" };
				info[Opcode::DI]      = { 4_u16, 1_u16, "DI" };
				info[Opcode::EI]      = { 4_u16, 1_u16, "EI" };
				info[Opcode::HALT]    = { 4_u16, 1_u16, "HALT" };
				info[Opcode::STOP]    = { 4_u16, 1_u16, "STOP" };
				info[Opcode::EXT]     = { 4_u16, 1_u16, "EXT" };
				info[Opcode::LD_A_A]  = { 4_u16, 1_u16, "LD A,A" };
				info[Opcode::LD_A_B]  = { 4_u16, 1_u16, "LD A,B" };
				info[Opcode::LD_A_C]  = { 4_u16, 1_u16, "LD A,C" };
				info[Opcode::LD_A_D]  = { 4_u16, 1_u16, "LD A,D" };
				info[Opcode::LD_A_E]  = { 4_u16, 1_u16, "LD A,E" };
				info[Opcode::LD_A_H]  = { 4_u16, 1_u16, "LD A,H" };
				info[Opcode::LD_A_L]  = { 4_u16, 1_u16, "LD A,L" };
				info[Opcode::LD_B_A]  = { 4_u16, 1_u16, "LD B,A" };
				info[Opcode::LD_B_B]  = { 4_u16, 1_u16, "LD B,B" };
				info[Opcode::LD_B_C]  = { 4_u16, 1_u16, "LD B,C" };
				info[Opcode::LD_B_D]  = { 4_u16, 1_u16, "LD B,D" };
				info[Opcode::LD_B_E]  = { 4_u16, 1_u16, "LD B,E" };
				info[Opcode::LD_B_H]  = { 4_u16, 1_u16, "LD B,H" };
				info[Opcode::LD_B_L]  = { 4_u16, 1_u16, "LD B,L" };
				info[Opcode::LD_C_A]  = { 4_u16, 1_u16, "LD C,A" };
				info[Opcode::LD_C_B]  = { 4_u16, 1_u16, "LD C,B" };
				info[Opcode::LD_C_C]  = { 4_u16, 1_u16, "LD C,C" };
				info[Opcode::LD_C_D]  = { 4_u16, 1_u16, "LD C,D" };
				info[Opcode::LD_C_E]  = { 4_u16, 1_u16, "LD C,E" };
				info[Opcode::LD_C_H]  = { 4_u16, 1_u16, "LD C,H" };
				info[Opcode::LD_C_L]  = { 4_u16, 1_u16, "LD C,L" };
				info[Opcode::LD_D_A]  = { 4_u16, 1_u16, "LD D,A" };
				info[Opcode::LD_D_B]  = { 4_u16, 1_u16, "LD D,B" };
				info[Opcode::LD_D_C]  = { 4_u16, 1_u16, "LD D,C" };
				info[Opcode::LD_D_D]  = { 4_u16, 1_u16, "LD D,D" };
				info[Opcode::LD_D_E]  = { 4_u16, 1_u16, "LD D,E" };
				info[Opcode::LD_D_H]  = { 4_u16, 1_u16, "LD D,H" };
				info[Opcode::LD_D_L]  = { 4_u16, 1_u16, "LD D,L" };
				info[Opcode::LD_E_A]  = { 4_u16, 1_u16, "LD E,A" };
				info[Opcode::LD_E_B]  = { 4_u16, 1_u16, "LD E,B" };
				info[Opcode::LD_E_C]  = { 4_u16, 1_u16, "LD E,C" };
				info[Opcode::LD_E_D]  = { 4_u16, 1_u16, "LD E,D" };
				info[Opcode::LD_E_E]  = { 4_u16, 1_u16, "LD E,E" };
				info[Opcode::LD_E_H]  = { 4_u16, 1_u16, "LD E,H" };
				info[Opcode::LD_E_L]  = { 4_u16, 1_u16, "LD E,L" };
				info[Opcode::LD_H_A]  = { 4_u16, 1_u16, "LD H,A" };
				info[Opcode::LD_H_B]  = { 4_u16, 1_u16, "LD H,B" };
				info[Opcode::LD_H_C]  = { 4_u16, 1_u16, "LD H,C" };
				info[Opcode::LD_H_D]  = { 4_u16, 1_u16, "LD H,D" };
				info[Opcode::LD_H_E]  = { 4_u16, 1_u16, "LD H,E" };
				info[Opcode::LD_H_H]  = { 4_u16, 1_u16, "LD H,H" };
				info[Opcode::LD_H_L]  = { 4_u16, 1_u16, "LD H,L" };
				info[Opcode::LD_L_A]  = { 4_u16, 1_u16, "LD L,A" };
				info[Opcode::LD_L_B]  = { 4_u16, 1_u16, "LD L,B" };
				info[Opcode::LD_L_C]  = { 4_u16, 1_u16, "LD L,C" };
				info[Opcode::LD_L_D]  = { 4_u16, 1_u16, "LD L,D" };
				info[Opcode::LD_L_E]  = { 4_u16, 1_u16, "LD L,E" };
				info[Opcode::LD_L_H]  = { 4_u16, 1_u16, "LD L,H" };
				info[Opcode::LD_L_L]  = { 4_u16, 1_u16, "LD L,L" };
				info[Opcode::ADD_A_A] = { 4_u16, 1_u16, "ADD A,A" };
				info[Opcode::ADD_A_B] = { 4_u16, 1_u16, "ADD A,B" };
				info[Opcode::ADD_A_C] = { 4_u16, 1_u16, "ADD A,C" };
				info[Opcode::ADD_A_D] = { 4_u16, 1_u16, "ADD A,D" };
				info[Opcode::ADD_A_E] = { 4_u16, 1_u16, "ADD A,E" };
				info[Opcode::ADD_A_H] = { 4_u16, 1_u16, "ADD A,H" };
				info[Opcode::ADD_A_L] = { 4_u16, 1_u16, "ADD A,L" };
				info[Opcode::ADC_A_A] = { 4_u16, 1_u16, "ADC A,A" };
				info[Opcode::ADC_A_B] = { 4_u16, 1_u16, "ADC A,B" };
				info[Opcode::ADC_A_C] = { 4_u16, 1_u16, "ADC A,C" };
				info[Opcode::ADC_A_D] = { 4_u16, 1_u16, "ADC A,D" };
				info[Opcode::ADC_A_E] = { 4_u16, 1_u16, "ADC A,E" };
				info[Opcode::ADC_A_H] = { 4_u16, 1_u16, "ADC A,H" };
				info[Opcode::ADC_A_L] = { 4_u16, 1_u16, "ADC A,L" };
				info[Opcode::SUB_A_A] = { 4_u16, 1_u16, "SUB A,A" };
				info[Opcode::SUB_A_B] = { 4_u16, 1_u16, "SUB A,B" };
				info[Opcode::SUB_A_C] = { 4_u16, 1_u16, "SUB A,C" };
				info[Opcode::SUB_A_D] = { 4_u16, 1_u16, "SUB A,D" };
				info[Opcode::SUB_A_E] = { 4_u16, 1_u16, "SUB A,E" };
				info[Opcode::SUB_A_H] = { 4_u16, 1_u16, "SUB A,H" };
				info[Opcode::SUB_A_L] = { 4_u16, 1_u16, "SUB A,L" };
				info[Opcode::SBC_A_A] = { 4_u16, 1_u16, "SBC A,A" };
				info[Opcode::SBC_A_B] = { 4_u16, 1_u16, "SBC A,B" };
				info[Opcode::SBC_A_C] = { 4_u16, 1_u16, "SBC A,C" };
				info[Opcode::SBC_A_D] = { 4_u16, 1_u16, "SBC A,D" };
				info[Opcode::SBC_A_E] = { 4_u16, 1_u16, "SBC A,E" };
				info[Opcode::SBC_A_H] = { 4_u16, 1_u16, "SBC A,H" };
				info[Opcode::SBC_A_L] = { 4_u16, 1_u16, "SBC A,L" };
				info[Opcode::AND_A_A] = { 4_u16, 1_u16, "AND A,A" };
				info[Opcode::AND_A_B] = { 4_u16, 1_u16, "AND A,B" };
				info[Opcode::AND_A_C] = { 4_u16, 1_u16, "AND A,C" };
				info[Opcode::AND_A_D] = { 4_u16, 1_u16, "AND A,D" };
				info[Opcode::AND_A_E] = { 4_u16, 1_u16, "AND A,E" };
				info[Opcode::AND_A_H] = { 4_u16, 1_u16, "AND A,H" };
				info[Opcode::AND_A_L] = { 4_u16, 1_u16, "AND A,L" };
				info[Opcode::OR_A_A]  = { 4_u16, 1_u16, "OR A,A" };
				info[Opcode::OR_A_B]  = { 4_u16, 1_u16, "OR A,B" };
				info[Opcode::OR_A_C]  = { 4_u16, 1_u16, "OR A,C" };
				info[Opcode::OR_A_D]  = { 4_u16, 1_u16, "OR A,D" };
				info[Opcode::OR_A_E]  = { 4_u16, 1_u16, "OR A,E" };
				info[Opcode::OR_A_H]  = { 4_u16, 1_u16, "OR A,H" };
				info[Opcode::OR_A_L]  = { 4_u16, 1_u16, "OR A,L" };
				info[Opcode::XOR_A_A] = { 4_u16, 1_u16, "XOR A,A" };
				info[Opcode::XOR_A_B] = { 4_u16, 1_u16, "XOR A,B" };
				info[Opcode::XOR_A_C] = { 4_u16, 1_u16, "XOR A,C" };
				info[Opcode::XOR_A_D] = { 4_u16, 1_u16, "XOR A,D" };
				info[Opcode::XOR_A_E] = { 4_u16, 1_u16, "XOR A,E" };
				info[Opcode::XOR_A_H] = { 4_u16, 1_u16, "XOR A,H" };
				info[Opcode::XOR_A_L] = { 4_u16, 1_u16, "XOR A,L" };
				info[Opcode::CP_A_A]  = { 4_u16, 1_u16, "CP A,A" };
				info[Opcode::CP_A_B]  = { 4_u16, 1_u16, "CP A,B" };
				info[Opcode::CP_A_C]  = { 4_u16, 1_u16, "CP A,C" };
				info[Opcode::CP_A_D]  = { 4_u16, 1_u16, "CP A,D" };
				info[Opcode::CP_A_E]  = { 4_u16, 1_u16, "CP A,E" };
				info[Opcode::CP_A_H]  = { 4_u16, 1_u16, "CP A,H" };
				info[Opcode::CP_A_L]  = { 4_u16, 1_u16, "CP A,L" };
				info[Opcode::INC_A]   = { 4_u16, 1_u16, "INC A" };
				info[Opcode::INC_B]   = { 4_u16, 1_u16, "INC B" };
				info[Opcode::INC_C]   = { 4_u16, 1_u16, "INC C" };
				info[Opcode::INC_D]   = { 4_u16, 1_u16, "INC D" };
				info[Opcode::INC_E]   = { 4_u16, 1_u16, "INC E" };
				info[Opcode::INC_H]   = { 4_u16, 1_u16, "INC H" };
				info[Opcode::INC_L]   = { 4_u16, 1_u16, "INC L" };
				info[Opcode::DEC_A]   = { 4_u16, 1_u16, "DEC A" };
				info[Opcode::DEC_B]   = { 4_u16, 1_u16, "DEC B" };
				info[Opcode::DEC_C]   = { 4_u16, 1_u16, "DEC C" };
				info[Opcode::DEC_D]   = { 4_u16, 1_u16, "DEC D" };
				info[Opcode::DEC_E]   = { 4_u16, 1_u16, "DEC E" };
				info[Opcode::DEC_H]   = { 4_u16, 1_u16, "DEC H" };
				info[Opcode::DEC_L]   = { 4_u16, 1_u16, "DEC L" };
				info[Opcode::DA_A]    = { 4_u16, 1_u16, "DA A" };
				info[Opcode::CPL_A]   = { 4_u16, 1_u16, "CPL A" };
				info[Opcode::CCF]     = { 4_u16, 1_u16, "CCF" };
				info[Opcode::SCF]     = { 4_u16, 1_u16, "SCF" };
				info[Opcode::JP_HL]   = { 4_u16, 1_u16, "JP HL" };
				info[Opcode::RLC_A]   = { 4_u16, 1_u16, "RLC A" };
				info[Opcode::RL_A]    = { 4_u16, 1_u16, "RL A" };
				info[Opcode::RRC_A]   = { 4_u16, 1_u16, "RRC A" };
				info[Opcode::RR_A]    = { 4_u16, 1_u16, "RR A" };

				// 8 cycles, 1 byte
				info[Opcode::LD_A_aBC]  = { 8_u16, 1_u16, "LD A,(BC)" };
				info[Opcode::LD_A_aDE]  = { 8_u16, 1_u16, "LD A,(DE)" };
				info[Opcode::LD_A_aHL]  = { 8_u16, 1_u16, "LD A,(HL)" };
				info[Opcode::LD_A_aFFC] = { 8_u16, 1_u16, "LD A,(FF00+C)" };
				info[Opcode::LD_B_aHL]  = { 8_u16, 1_u16, "LD B,(HL)" };
				info[Opcode::LD_C_aHL]  = { 8_u16, 1_u16, "LD C,(HL)" };
				info[Opcode::LD_D_aHL]  = { 8_u16, 1_u16, "LD D,(HL)" };
				info[Opcode::LD_E_aHL]  = { 8_u16, 1_u16, "LD E,(HL)" };
				info[Opcode::LD_H_aHL]  = { 8_u16, 1_u16, "LD H,(HL)" };
				info[Opcode::LD_L_aHL]  = { 8_u16, 1_u16, "LD L,(HL)" };
				info[Opcode::LD_aHL_A]  = { 8_u16, 1_u16, "LD (HL),A" };
				info[Opcode::LD_aHL_B]  = { 8_u16, 1_u16, "LD (HL),B" };
				info[Opcode::LD_aHL_C]  = { 8_u16, 1_u16, "LD (HL),C" };
				info[Opcode::LD_aHL_D]  = { 8_u16, 1_u16, "LD (HL),D" };
				info[Opcode::LD_aHL_E]  = { 8_u16, 1_u16, "LD (HL),E" };
				info[Opcode::LD_aHL_H]  = { 8_u16, 1_u16, "LD (HL),H" };
				info[Opcode::LD_aHL_L]  = { 8_u16, 1_u16, "LD (HL),L" };
				info[Opcode::LD_aBC_A]  = { 8_u16, 1_u16, "LD (BC),A" };
				info[Opcode::LD_aDE_A]  = { 8_u16, 1_u16, "LD (DE),A" };
				info[Opcode::LD_aFFC_A] = { 8_u16, 1_u16, "LD (FF00+C),A" };
				info[Opcode::LD_SP_HL]  = { 8_u16, 1_u16, "LD SP,HL" };
				info[Opcode::LDI_A_aHL] = { 8_u16, 1_u16, "LDI A,(HL)" };
				info[Opcode::LDD_A_aHL] = { 8_u16, 1_u16, "LDD A,(HL)" };
				info[Opcode::LDI_aHL_A] = { 8_u16, 1_u16, "LDI (HL),A" };
				info[Opcode::LDD_aHL_A] = { 8_u16, 1_u16, "LDD (HL),A" };
				info[Opcode::ADD_A_aHL] = { 8_u16, 1_u16, "ADD A,(HL)" };
				info[Opcode::ADD_HL_BC] = { 8_u16, 1_u16, "ADD HL,BC" };
				info[Opcode::ADD_HL_DE] = { 8_u16, 1_u16, "ADD HL,DE" };
				info[Opcode::ADD_HL_HL] = { 8_u16, 1_u16, "ADD HL,HL" };
				info[Opcode::ADD_HL_SP] = { 8_u16, 1_u16, "ADD HL,SP" };
				info[Opcode::ADC_A_aHL] = { 8_u16, 1_u16, "ADC A,(HL)" };
				info[Opcode::SUB_A_aHL] = { 8_u16, 1_u16, "SUB A,(HL)" };
				info[Opcode::SBC_A_aHL] = { 8_u16, 1_u16, "SBC A,(HL)" };
				info[Opcode::AND_A_aHL] = { 8_u16, 1_u16, "AND A,(HL)" };
				info[Opcode::OR_A_aHL]  = { 8_u16, 1_u16, "OR A,(HL)" };
				info[Opcode::XOR_A_aHL] = { 8_u16, 1_u16, "XOR A,(HL)" };
				info[Opcode::CP_A_aHL]  = { 8_u16, 1_u16, "CP A,(HL)" };
				info[Opcode::INC_BC]    = { 8_u16, 1_u16, "INC BC" };
				info[Opcode::INC_DE]    = { 8_u16, 1_u16, "INC DE" };
				info[Opcode::INC_HL]    = { 8_u16, 1_u16, "INC HL" };
				info[Opcode::INC_SP]    = { 8_u16, 1_u16, "INC SP" };
				info[Opcode::DEC_BC]    = { 8_u16, 1_u16, "DEC BC" };
				info[Opcode::DEC_DE]    = { 8_u16, 1_u16, "DEC DE" };
				info[Opcode::DEC_HL]    = { 8_u16, 1_u16, "DEC HL" };
				info[Opcode::DEC_SP]    = { 8_u16, 1_u16, "DEC SP" };
				info[Opcode::RET]       = { 8_u16, 1_u16, "RET" };
				info[Opcode::RET_NZ]    = { 8_u16, 1_u16, "RET NZ" };
				info[Opcode::RET_Z]     = { 8_u16, 1_u16, "RET Z" };
				info[Opcode::RET_NC]    = { 8_u16, 1_u16, "RET NC" };
				info[Opcode::RET_C]     = { 8_u16, 1_u16, "RET C" };
				info[Opcode::RETI]      = { 8_u16, 1_u16, "RETI" };

				// 8 cycles, 2 bytes
				info[Opcode::LD_A_n] =  { 8_u16, 2_u16, "LD A,n" };
				info[Opcode::LD_B_n] =  { 8_u16, 2_u16, "LD B,n" };
				info[Opcode::LD_C_n]  = { 8_u16, 2_u16, "LD C,n" };
				info[Opcode::LD_D_n]  = { 8_u16, 2_u16, "LD D,n" };
				info[Opcode::LD_E_n]  = { 8_u16, 2_u16, "LD E,n" };
				info[Opcode::LD_H_n]  = { 8_u16, 2_u16, "LD H,n" };
				info[Opcode::LD_L_n]  = { 8_u16, 2_u16, "LD L,n" };
				info[Opcode::ADD_A_n] = { 8_u16, 2_u16, "ADD A,n" };
				info[Opcode::ADC_A_n] = { 8_u16, 2_u16, "ADC A,n" };
				info[Opcode::SUB_A_n] = { 8_u16, 2_u16, "SUB A,n" };
				info[Opcode::SBC_A_n] = { 8_u16, 2_u16, "SBC A,n" };
				info[Opcode::AND_A_n] = { 8_u16, 2_u16, "ADD A,n" };
				info[Opcode::OR_A_n]  = { 8_u16, 2_u16, "OR A,n" };
				info[Opcode::XOR_A_n] = { 8_u16, 2_u16, "XOR A,n" };
				info[Opcode::CP_A_n]  = { 8_u16, 2_u16, "CP A,n" };
				info[Opcode::JR_n]    = { 8_u16, 2_u16, "JR n" };
				info[Opcode::JR_NZ_n] = { 8_u16, 2_u16, "JR NZ,n" };
				info[Opcode::JR_Z_n]  = { 8_u16, 2_u16, "JR Z,n" };
				info[Opcode::JR_NC_n] = { 8_u16, 2_u16, "JR NC,n" };
				info[Opcode::JR_C_n]  = { 8_u16, 2_u16, "JR C,n" };

				// 12 cycles, 1 byte
				info[Opcode::POP_AF] = { 12_u16, 1_u16, "POP AF" };
				info[Opcode::POP_BC] = { 12_u16, 1_u16, "POP BC" };
				info[Opcode::POP_DE] = { 12_u16, 1_u16, "POP DE" };
				info[Opcode::POP_HL] = { 12_u16, 1_u16, "POP HL" };

				// 12 cycles, 2 bytes
				info[Opcode::LD_A_aFFn] = { 12_u16, 2_u16, "LD A,(FF00+n)" };
				info[Opcode::LD_aHL_n]  = { 12_u16, 2_u16, "LD (HL),n" };
				info[Opcode::LD_aFFn_A] = { 12_u16, 2_u16, "LD (FF00+n),A" };
				info[Opcode::LD_HL_SPn] = { 12_u16, 2_u16, "LD HL,SP+n" };
				info[Opcode::INC_aHL]   = { 12_u16, 2_u16, "INC (HL)" };
				info[Opcode::DEC_aHL]   = { 12_u16, 2_u16, "DEC (HL)" };

				// 12 cycles, 3 bytes
				info[Opcode::LD_BC_nn]   = { 12_u16, 3_u16, "LD BC,nn" };
				info[Opcode::LD_DE_nn]   = { 12_u16, 3_u16, "LD DE,nn" };
				info[Opcode::LD_HL_nn]   = { 12_u16, 3_u16, "LD HL,nn" };
				info[Opcode::LD_SP_nn]   = { 12_u16, 3_u16, "LD SP,nn" };
				info[Opcode::JP_nn]      = { 12_u16, 3_u16, "JP nn" };
				info[Opcode::JP_NZ_nn]   = { 12_u16, 3_u16, "JP NZ nn" };
				info[Opcode::JP_Z_nn]    = { 12_u16, 3_u16, "JP Z nn" };
				info[Opcode::JP_NC_nn]   = { 12_u16, 3_u16, "JP NC nn" };
				info[Opcode::JP_C_nn]    = { 12_u16, 3_u16, "JP C nn" };
				info[Opcode::CALL_nn]    = { 12_u16, 3_u16, "CALL nn" };
				info[Opcode::CALL_NZ_nn] = { 12_u16, 3_u16, "CALL NZ nn" };
				info[Opcode::CALL_Z_nn]  = { 12_u16, 3_u16, "CALL Z nn" };
				info[Opcode::CALL_NC_nn] = { 12_u16, 3_u16, "CALL NC nn" };
				info[Opcode::CALL_C_nn]  = { 12_u16, 3_u16, "CALL C nn" };

				// 16 cycles, 1 byte
				info[Opcode::PUSH_AF] = { 16_u16, 1_u16, "PUSH AF" };
				info[Opcode::PUSH_BC] = { 16_u16, 1_u16, "PUSH BC" };
				info[Opcode::PUSH_DE] = { 16_u16, 1_u16, "PUSH DE" };
				info[Opcode::PUSH_HL] = { 16_u16, 1_u16, "PUSH HL" };

				// 16 cycles, 2 bytes
				info[Opcode::ADD_SP_n] = { 16_u16, 2_u16, "ADD SP,n" };

				// 16 cycles, 3 bytes
				info[Opcode::LD_A_ann] = { 16_u16, 3_u16, "LD A,(nn)" };
				info[Opcode::LD_ann_A] = { 16_u16, 3_u16, "LD (nn),A" };

				// 20 cycles, 3 bytes
				info[Opcode::LD_ann_SP] = { 20_u16, 3_u16, "LD (nn),SP" };

				// 32 cycles, 1 byte
				info[Opcode::RST_00] = { 32_u16, 1_u16, "RST 00" };
				info[Opcode::RST_08] = { 32_u16, 1_u16, "RST 08" };
				info[Opcode::RST_10] = { 32_u16, 1_u16, "RST 10" };
				info[Opcode::RST_18] = { 32_u16, 1_u16, "RST 18" };
				info[Opcode::RST_20] = { 32_u16, 1_u16, "RST 20" };
				info[Opcode::RST_28] = { 32_u16, 1_u16, "RST 28" };
				info[Opcode::RST_30] = { 32_u16, 1_u16, "RST 30" };
				info[Opcode::RST_38] = { 32_u16, 1_u16, "RST 38" };

				return info;
			}();
		}

		constexpr std::optional<uint16_t> GetCycles(Opcode::Enum a_Opcode)
		{
			return Internal::g_OpcodeInfo[a_Opcode].m_Cycles;
		}

		constexpr std::optional<uint16_t> GetSize(Opcode::Enum a_Opcode)
		{
			return Internal::g_OpcodeInfo[a_Opcode].m_Size;
		}

		constexpr std::optional<std::string_view> ToString(Opcode::Enum a_Opcode)
		{
			return Internal::g_OpcodeInfo[a_Opcode].m_Name;
		}
	}
}

#endif