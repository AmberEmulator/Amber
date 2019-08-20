#ifndef H_DEMU_GAMEBOY_INSTRUCTION
#define H_DEMU_GAMEBOY_INSTRUCTION

#include <gameboy/api.hpp>

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

		// TODO: optional
		constexpr std::optional<uint16_t> GetCycleCount(Instruction::Enum a_Instruction)
		{
			switch (a_Instruction)
			{
				case Instruction::NOP:
				case Instruction::DI:
				case Instruction::EI:
				case Instruction::HALT:
				case Instruction::STOP:
				case Instruction::LD_A_A:
				case Instruction::LD_A_B:
				case Instruction::LD_A_C:
				case Instruction::LD_A_D:
				case Instruction::LD_A_E:
				case Instruction::LD_A_H:
				case Instruction::LD_A_L:
				case Instruction::LD_B_A:
				case Instruction::LD_B_B:
				case Instruction::LD_B_C:
				case Instruction::LD_B_D:
				case Instruction::LD_B_E:
				case Instruction::LD_B_H:
				case Instruction::LD_B_L:
				case Instruction::LD_C_A:
				case Instruction::LD_C_B:
				case Instruction::LD_C_C:
				case Instruction::LD_C_D:
				case Instruction::LD_C_E:
				case Instruction::LD_C_H:
				case Instruction::LD_C_L:
				case Instruction::LD_D_A:
				case Instruction::LD_D_B:
				case Instruction::LD_D_C:
				case Instruction::LD_D_D:
				case Instruction::LD_D_E:
				case Instruction::LD_D_H:
				case Instruction::LD_D_L:
				case Instruction::LD_E_A:
				case Instruction::LD_E_B:
				case Instruction::LD_E_C:
				case Instruction::LD_E_D:
				case Instruction::LD_E_E:
				case Instruction::LD_E_H:
				case Instruction::LD_E_L:
				case Instruction::LD_H_A:
				case Instruction::LD_H_B:
				case Instruction::LD_H_C:
				case Instruction::LD_H_D:
				case Instruction::LD_H_E:
				case Instruction::LD_H_H:
				case Instruction::LD_H_L:
				case Instruction::LD_L_A:
				case Instruction::LD_L_B:
				case Instruction::LD_L_C:
				case Instruction::LD_L_D:
				case Instruction::LD_L_E:
				case Instruction::LD_L_H:
				case Instruction::LD_L_L:
				case Instruction::ADD_A_A:
				case Instruction::ADD_A_B:
				case Instruction::ADD_A_C:
				case Instruction::ADD_A_D:
				case Instruction::ADD_A_E:
				case Instruction::ADD_A_H:
				case Instruction::ADD_A_L:
				case Instruction::ADC_A_A:
				case Instruction::ADC_A_B:
				case Instruction::ADC_A_C:
				case Instruction::ADC_A_D:
				case Instruction::ADC_A_E:
				case Instruction::ADC_A_H:
				case Instruction::ADC_A_L:
				case Instruction::SUB_A_A:
				case Instruction::SUB_A_B:
				case Instruction::SUB_A_C:
				case Instruction::SUB_A_D:
				case Instruction::SUB_A_E:
				case Instruction::SUB_A_H:
				case Instruction::SUB_A_L:
				case Instruction::SBC_A_A:
				case Instruction::SBC_A_B:
				case Instruction::SBC_A_C:
				case Instruction::SBC_A_D:
				case Instruction::SBC_A_E:
				case Instruction::SBC_A_H:
				case Instruction::SBC_A_L:
				case Instruction::AND_A_A:
				case Instruction::AND_A_B:
				case Instruction::AND_A_C:
				case Instruction::AND_A_D:
				case Instruction::AND_A_E:
				case Instruction::AND_A_H:
				case Instruction::AND_A_L:
				case Instruction::OR_A_A:
				case Instruction::OR_A_B:
				case Instruction::OR_A_C:
				case Instruction::OR_A_D:
				case Instruction::OR_A_E:
				case Instruction::OR_A_H:
				case Instruction::OR_A_L:
				case Instruction::XOR_A_A:
				case Instruction::XOR_A_B:
				case Instruction::XOR_A_C:
				case Instruction::XOR_A_D:
				case Instruction::XOR_A_E:
				case Instruction::XOR_A_H:
				case Instruction::XOR_A_L:
				case Instruction::CP_A_A:
				case Instruction::CP_A_B:
				case Instruction::CP_A_C:
				case Instruction::CP_A_D:
				case Instruction::CP_A_E:
				case Instruction::CP_A_H:
				case Instruction::CP_A_L:
				case Instruction::INC_A:
				case Instruction::INC_B:
				case Instruction::INC_C:
				case Instruction::INC_D:
				case Instruction::INC_E:
				case Instruction::INC_H:
				case Instruction::INC_L:
				case Instruction::DEC_A:
				case Instruction::DEC_B:
				case Instruction::DEC_C:
				case Instruction::DEC_D:
				case Instruction::DEC_E:
				case Instruction::DEC_H:
				case Instruction::DEC_L:
				case Instruction::DA_A:
				case Instruction::CPL_A:
				case Instruction::CCF:
				case Instruction::SCF:
				case Instruction::JP_HL:
				case Instruction::RLC_A:
				case Instruction::RL_A:
				case Instruction::RRC_A:
				case Instruction::RR_A:
				return 4;

				case Instruction::LD_A_n:
				case Instruction::LD_A_aBC:
				case Instruction::LD_A_aDE:
				case Instruction::LD_A_aHL:
				case Instruction::LD_A_aFFC:
				case Instruction::LD_B_n:
				case Instruction::LD_B_aHL:
				case Instruction::LD_C_n:
				case Instruction::LD_C_aHL:
				case Instruction::LD_D_n:
				case Instruction::LD_D_aHL:
				case Instruction::LD_E_n:
				case Instruction::LD_E_aHL:
				case Instruction::LD_H_n:
				case Instruction::LD_H_aHL:
				case Instruction::LD_L_n:
				case Instruction::LD_L_aHL:
				case Instruction::LD_aHL_A:
				case Instruction::LD_aHL_B:
				case Instruction::LD_aHL_C:
				case Instruction::LD_aHL_D:
				case Instruction::LD_aHL_E:
				case Instruction::LD_aHL_H:
				case Instruction::LD_aHL_L:
				case Instruction::LD_aBC_A:
				case Instruction::LD_aDE_A:
				case Instruction::LD_aFFC_A:
				case Instruction::LD_SP_HL:
				case Instruction::LDI_A_aHL:
				case Instruction::LDD_A_aHL:
				case Instruction::LDI_aHL_A:
				case Instruction::LDD_aHL_A:
				case Instruction::ADD_A_n:
				case Instruction::ADD_A_aHL:
				case Instruction::ADD_HL_BC:
				case Instruction::ADD_HL_DE:
				case Instruction::ADD_HL_HL:
				case Instruction::ADD_HL_SP:
				case Instruction::ADC_A_n:
				case Instruction::ADC_A_aHL:
				case Instruction::SUB_A_n:
				case Instruction::SUB_A_aHL:
				case Instruction::SBC_A_n:
				case Instruction::SBC_A_aHL:
				case Instruction::AND_A_n:
				case Instruction::AND_A_aHL:
				case Instruction::OR_A_n:
				case Instruction::OR_A_aHL:
				case Instruction::XOR_A_n:
				case Instruction::XOR_A_aHL:
				case Instruction::CP_A_n:
				case Instruction::CP_A_aHL:
				case Instruction::INC_BC:
				case Instruction::INC_DE:
				case Instruction::INC_HL:
				case Instruction::INC_SP:
				case Instruction::DEC_BC:
				case Instruction::DEC_DE:
				case Instruction::DEC_HL:
				case Instruction::DEC_SP:
				case Instruction::JR_n:
				case Instruction::JR_NZ_n:
				case Instruction::JR_Z_n:
				case Instruction::JR_NC_n:
				case Instruction::JR_C_n:
				case Instruction::RET:
				case Instruction::RET_NZ:
				case Instruction::RET_Z:
				case Instruction::RET_NC:
				case Instruction::RET_C:
				case Instruction::RETI:
				return 8;

				case Instruction::LD_A_aFFn:
				case Instruction::LD_aHL_n:
				case Instruction::LD_aFFn_A:
				case Instruction::LD_BC_nn:
				case Instruction::LD_DE_nn:
				case Instruction::LD_HL_nn:
				case Instruction::LD_HL_SPn:
				case Instruction::LD_SP_nn:
				case Instruction::POP_AF:
				case Instruction::POP_BC:
				case Instruction::POP_DE:
				case Instruction::POP_HL:
				case Instruction::INC_aHL:
				case Instruction::DEC_aHL:
				case Instruction::JP_nn:
				case Instruction::JP_NZ_nn:
				case Instruction::JP_Z_nn:
				case Instruction::JP_NC_nn:
				case Instruction::JP_C_nn:
				case Instruction::CALL_nn:
				case Instruction::CALL_NZ_nn:
				case Instruction::CALL_Z_nn:
				case Instruction::CALL_NC_nn:
				case Instruction::CALL_C_nn:
				return 12;

				case Instruction::LD_A_ann:
				case Instruction::LD_ann_A:
				case Instruction::PUSH_AF:
				case Instruction::PUSH_BC:
				case Instruction::PUSH_DE:
				case Instruction::PUSH_HL:
				case Instruction::ADD_SP_n:
				return 16;

				case Instruction::LD_ann_SP:
				return 20;

				case Instruction::RST_00:
				case Instruction::RST_08:
				case Instruction::RST_10:
				case Instruction::RST_18:
				case Instruction::RST_20:
				case Instruction::RST_28:
				case Instruction::RST_30:
				case Instruction::RST_38:
				return 32;

				default:
				return {};
			}
		}

		constexpr std::optional<std::string_view> ToString(Instruction::Enum a_Instruction)
		{
			switch (a_Instruction)
			{
				case Instruction::NOP:       return "NOP";
				case Instruction::DI:        return "DI";
				case Instruction::EI:        return "EI";
				case Instruction::HALT:      return "HALT";
				case Instruction::STOP:      return "STOP";
				case Instruction::EXT:       return "EXT";
				
				case Instruction::LD_A_A:  	 return "LD A,A";
				case Instruction::LD_A_B:    return "LD A,B";
				case Instruction::LD_A_C:    return "LD A,C";
				case Instruction::LD_A_D:    return "LD A,D";
				case Instruction::LD_A_E:    return "LD A,E";
				case Instruction::LD_A_H:    return "LD A,H";
				case Instruction::LD_A_L:    return "LD A,L";
				case Instruction::LD_A_n:    return "LD A,n";
				case Instruction::LD_A_aBC:  return "LD A,(BC)";
				case Instruction::LD_A_aDE:  return "LD A,(DE)";
				case Instruction::LD_A_aHL:  return "LD A,(HL)";
				case Instruction::LD_A_ann:  return "LD A,(nn)";
				case Instruction::LD_A_aFFC: return "LD A,(FF00+C)";
				case Instruction::LD_A_aFFn: return "LD A,(FF00+n)";

				case Instruction::LD_B_A:	 return "LD B,A";
				case Instruction::LD_B_B:	 return "LD B,B";
				case Instruction::LD_B_C:	 return "LD B,C";
				case Instruction::LD_B_D:	 return "LD B,D";
				case Instruction::LD_B_E:	 return "LD B,E";
				case Instruction::LD_B_H:	 return "LD B,H";
				case Instruction::LD_B_L:	 return "LD B,L";
				case Instruction::LD_B_n:    return "LD B,n";
				case Instruction::LD_B_aHL:  return "LD B,(HL)";

				case Instruction::LD_C_A:	 return "LD C,A";
				case Instruction::LD_C_B:	 return "LD C,B";
				case Instruction::LD_C_C:	 return "LD C,C";
				case Instruction::LD_C_D:	 return "LD C,D";
				case Instruction::LD_C_E:	 return "LD C,E";
				case Instruction::LD_C_H:	 return "LD C,H";
				case Instruction::LD_C_L:	 return "LD C,L";
				case Instruction::LD_C_n:    return "LD C,n";
				case Instruction::LD_C_aHL:  return "LD C,(HL)";

				case Instruction::LD_D_A:    return "LD D,A";
				case Instruction::LD_D_B:    return "LD D,B";
				case Instruction::LD_D_C:    return "LD D,C";
				case Instruction::LD_D_D:    return "LD D,D";
				case Instruction::LD_D_E:    return "LD D,E";
				case Instruction::LD_D_H:    return "LD D,H";
				case Instruction::LD_D_L:    return "LD D,L";
				case Instruction::LD_D_n:    return "LD D,n";
				case Instruction::LD_D_aHL:  return "LD D,(HL)";

				case Instruction::LD_E_A:    return "LD E,A";
				case Instruction::LD_E_B:    return "LD E,B";
				case Instruction::LD_E_C:    return "LD E,C";
				case Instruction::LD_E_D:    return "LD E,D";
				case Instruction::LD_E_E:    return "LD E,E";
				case Instruction::LD_E_H:    return "LD E,H";
				case Instruction::LD_E_L:    return "LD E,L";
				case Instruction::LD_E_n:    return "LD E,n";
				case Instruction::LD_E_aHL:  return "LD E,(HL)";

				case Instruction::LD_H_A:    return "LD H,A";
				case Instruction::LD_H_B:    return "LD H,B";
				case Instruction::LD_H_C:    return "LD H,C";
				case Instruction::LD_H_D:    return "LD H,D";
				case Instruction::LD_H_E:    return "LD H,E";
				case Instruction::LD_H_H:    return "LD H,H";
				case Instruction::LD_H_L:    return "LD H,L";
				case Instruction::LD_H_n:    return "LD H,n";
				case Instruction::LD_H_aHL:  return "LD H,(HL)";

				case Instruction::LD_L_A:    return "LD L,A";
				case Instruction::LD_L_B:    return "LD L,B";
				case Instruction::LD_L_C:    return "LD L,C";
				case Instruction::LD_L_D:    return "LD L,D";
				case Instruction::LD_L_E:    return "LD L,E";
				case Instruction::LD_L_H:    return "LD L,H";
				case Instruction::LD_L_L:    return "LD L,L";
				case Instruction::LD_L_n:    return "LD L,n";
				case Instruction::LD_L_aHL:  return "LD L,(HL)";

				case Instruction::LD_aHL_A:  return "LD (HL),A";
				case Instruction::LD_aHL_B:  return "LD (HL),B";
				case Instruction::LD_aHL_C:  return "LD (HL),C";
				case Instruction::LD_aHL_D:  return "LD (HL),D";
				case Instruction::LD_aHL_E:  return "LD (HL),E";
				case Instruction::LD_aHL_H:  return "LD (HL),H";
				case Instruction::LD_aHL_L:  return "LD (HL),L";
				case Instruction::LD_aHL_n:  return "LD (HL),n";

				case Instruction::LD_aBC_A:  return "LD (BC),A";
				case Instruction::LD_aDE_A:  return "LD (DE),A";
				case Instruction::LD_ann_A:  return "LD (nn),A";
				case Instruction::LD_aFFC_A: return "LD (FF00+C),A";
				case Instruction::LD_aFFn_A: return "LD (FF00+n),A";

				case Instruction::LDI_A_aHL: return "LDI A,(HL)";
				case Instruction::LDD_A_aHL: return "LDD A,(HL)";
				case Instruction::LDI_aHL_A: return "LDI (HL),A";
				case Instruction::LDD_aHL_A: return "LDD (HL),A";

				case Instruction::LD_BC_nn:  return "LD BC,nn";
				case Instruction::LD_DE_nn:  return "LD DE,nn";
				case Instruction::LD_HL_nn:  return "LD HL,nn";
				case Instruction::LD_HL_SPn: return "LD HL,SP+n";
				case Instruction::LD_SP_nn:  return "LD SP,nn";
				case Instruction::LD_SP_HL:  return "LD SP,HL";
				case Instruction::LD_ann_SP: return "LD (nn),SP";

				case Instruction::PUSH_AF:   return "PUSH AF";
				case Instruction::PUSH_BC:   return "PUSH BC";
				case Instruction::PUSH_DE:   return "PUSH DE";
				case Instruction::PUSH_HL:   return "PUSH HL";

				case Instruction::POP_AF:    return "POP AF";
				case Instruction::POP_BC:    return "POP BC";
				case Instruction::POP_DE:    return "POP DE";
				case Instruction::POP_HL:    return "POP HL";

				case Instruction::ADD_A_A:   return "ADD A,A";
				case Instruction::ADD_A_B:   return "ADD A,B";
				case Instruction::ADD_A_C:   return "ADD A,C";
				case Instruction::ADD_A_D:   return "ADD A,D";
				case Instruction::ADD_A_E:   return "ADD A,E";
				case Instruction::ADD_A_H:   return "ADD A,H";
				case Instruction::ADD_A_L:   return "ADD A,L";
				case Instruction::ADD_A_n:   return "ADD A,n";
				case Instruction::ADD_A_aHL: return "ADD A,(HL)";
				case Instruction::ADD_HL_BC: return "ADD HL,BC";
				case Instruction::ADD_HL_DE: return "ADD HL,DE";
				case Instruction::ADD_HL_HL: return "ADD HL,HL";
				case Instruction::ADD_HL_SP: return "ADD HL,SP";
				case Instruction::ADD_SP_n:  return "ADD SP,n";

				case Instruction::ADC_A_A:   return "ADC A,A";
				case Instruction::ADC_A_B:   return "ADC A,B";
				case Instruction::ADC_A_C:   return "ADC A,C";
				case Instruction::ADC_A_D:   return "ADC A,D";
				case Instruction::ADC_A_E:   return "ADC A,E";
				case Instruction::ADC_A_H:   return "ADC A,H";
				case Instruction::ADC_A_L:   return "ADC A,L";
				case Instruction::ADC_A_n:   return "ADC A,n";
				case Instruction::ADC_A_aHL: return "ADC A,(HL)";

				case Instruction::SUB_A_A:   return "SUB A,A";
				case Instruction::SUB_A_B:   return "SUB A,B";
				case Instruction::SUB_A_C:   return "SUB A,C";
				case Instruction::SUB_A_D:   return "SUB A,D";
				case Instruction::SUB_A_E:   return "SUB A,E";
				case Instruction::SUB_A_H:   return "SUB A,H";
				case Instruction::SUB_A_L:   return "SUB A,L";
				case Instruction::SUB_A_n:   return "SUB A,n";
				case Instruction::SUB_A_aHL: return "SUB A,(HL)";

				case Instruction::SBC_A_A:   return "SBC A,A";
				case Instruction::SBC_A_B:   return "SBC A,B";
				case Instruction::SBC_A_C:   return "SBC A,C";
				case Instruction::SBC_A_D:   return "SBC A,D";
				case Instruction::SBC_A_E:   return "SBC A,E";
				case Instruction::SBC_A_H:   return "SBC A,H";
				case Instruction::SBC_A_L:   return "SBC A,L";
				case Instruction::SBC_A_n:   return "SBC A,n";
				case Instruction::SBC_A_aHL: return "SBC A,(HL)";

				case Instruction::AND_A_A:   return "AND A,A";
				case Instruction::AND_A_B:   return "AND A,B";
				case Instruction::AND_A_C:   return "AND A,C";
				case Instruction::AND_A_D:   return "AND A,D";
				case Instruction::AND_A_E:   return "AND A,E";
				case Instruction::AND_A_H:   return "AND A,H";
				case Instruction::AND_A_L:   return "AND A,L";
				case Instruction::AND_A_n:   return "AND A,n";
				case Instruction::AND_A_aHL: return "AND A,(HL)";

				case Instruction::OR_A_A:    return "OR A,A";
				case Instruction::OR_A_B:    return "OR A,B";
				case Instruction::OR_A_C:    return "OR A,C";
				case Instruction::OR_A_D:    return "OR A,D";
				case Instruction::OR_A_E:    return "OR A,E";
				case Instruction::OR_A_H:    return "OR A,H";
				case Instruction::OR_A_L:    return "OR A,L";
				case Instruction::OR_A_n:    return "OR A,n";
				case Instruction::OR_A_aHL:  return "OR A,(HL)";

				case Instruction::XOR_A_A:   return "XOR A,A";
				case Instruction::XOR_A_B:   return "XOR A,B";
				case Instruction::XOR_A_C:   return "XOR A,C";
				case Instruction::XOR_A_D:   return "XOR A,D";
				case Instruction::XOR_A_E:   return "XOR A,E";
				case Instruction::XOR_A_H:   return "XOR A,H";
				case Instruction::XOR_A_L:   return "XOR A,L";
				case Instruction::XOR_A_n:   return "XOR A,n";
				case Instruction::XOR_A_aHL: return "XOR A,(HL)";

				case Instruction::CP_A_A:    return "CP A,A";
				case Instruction::CP_A_B:    return "CP A,B";
				case Instruction::CP_A_C:    return "CP A,C";
				case Instruction::CP_A_D:    return "CP A,D";
				case Instruction::CP_A_E:    return "CP A,E";
				case Instruction::CP_A_H:    return "CP A,H";
				case Instruction::CP_A_L:    return "CP A,L";
				case Instruction::CP_A_n:    return "CP A,n";
				case Instruction::CP_A_aHL:  return "CP A,(HL)";

				case Instruction::INC_A:     return "INC A";
				case Instruction::INC_B:     return "INC B";
				case Instruction::INC_C:     return "INC C";
				case Instruction::INC_D:     return "INC D";
				case Instruction::INC_E:     return "INC E";
				case Instruction::INC_H:     return "INC H";
				case Instruction::INC_L:     return "INC L";
				case Instruction::INC_aHL:   return "INC (HL)";
				case Instruction::INC_BC:    return "INC BC";
				case Instruction::INC_DE:    return "INC DE";
				case Instruction::INC_HL:    return "INC HL";
				case Instruction::INC_SP:    return "INC SP";

				case Instruction::DEC_A:     return "DEC A";
				case Instruction::DEC_B:     return "DEC B";
				case Instruction::DEC_C:     return "DEC C";
				case Instruction::DEC_D:     return "DEC D";
				case Instruction::DEC_E:     return "DEC E";
				case Instruction::DEC_H:     return "DEC H";
				case Instruction::DEC_L:     return "DEC L";
				case Instruction::DEC_aHL:   return "DEC (HL)";
				case Instruction::DEC_BC:    return "DEC BC";
				case Instruction::DEC_DE:    return "DEC DE";
				case Instruction::DEC_HL:    return "DEC HL";
				case Instruction::DEC_SP:    return "DEC SP";

				case Instruction::RLC_A:     return "RLC A";
				case Instruction::RL_A:      return "RL A";
				case Instruction::RRC_A:     return "RRC A";
				case Instruction::RR_A:      return "RR A";

				case Instruction::DA_A:      return "DA A";
				case Instruction::CPL_A:     return "CPL A";
				case Instruction::CCF:       return "CCF";
				case Instruction::SCF:       return "SCF";
					
				case Instruction::JP_nn:     return "JP nn";
				case Instruction::JP_NZ_nn:  return "JP NZ,nn";
				case Instruction::JP_Z_nn:   return "JP Z,nn";
				case Instruction::JP_NC_nn:  return "JP NC,nn";
				case Instruction::JP_C_nn:   return "JP C,nn";
				case Instruction::JP_HL:     return "JP HL";

				case Instruction::JR_n:      return "JR n";
				case Instruction::JR_NZ_n:   return "JR NZ,n";
				case Instruction::JR_Z_n:    return "JR Z,n";
				case Instruction::JR_NC_n:   return "JR NC,n";
				case Instruction::JR_C_n:    return "JR C,n";

				case Instruction::CALL_nn:    return "CALL nn";
				case Instruction::CALL_NZ_nn: return "CALL NZ,nn";
				case Instruction::CALL_Z_nn:  return "CALL Z,nn";
				case Instruction::CALL_NC_nn: return "CALL NC,nn";
				case Instruction::CALL_C_nn:  return "CALL C,nn";

				case Instruction::RET:        return "RET";
				case Instruction::RET_NZ:     return "RET NZ";
				case Instruction::RET_Z:      return "RET Z";
				case Instruction::RET_NC:     return "RET NC";
				case Instruction::RET_C:      return "RET C";
				case Instruction::RETI:       return "RETI";

				case Instruction::RST_00:     return "RST 0x00";
				case Instruction::RST_08:     return "RST 0x08";
				case Instruction::RST_10:     return "RST 0x10";
				case Instruction::RST_18:     return "RST 0x18";
				case Instruction::RST_20:     return "RST 0x20";
				case Instruction::RST_28:     return "RST 0x28";
				case Instruction::RST_30:     return "RST 0x30";
				case Instruction::RST_38:     return "RST 0x38";

				default:
				return {};
			}
		}
	}
}

#endif