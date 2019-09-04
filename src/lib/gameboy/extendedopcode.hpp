#ifndef H_AMBER_GAMEBOY_EXTENDEDINSTRUCTION
#define H_AMBER_GAMEBOY_EXTENDEDINSTRUCTION

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

namespace Amber::Gameboy
{
	namespace ExtendedInstruction
	{
		enum Enum : uint8_t
		{
			RLC_A = 0x07,
			RLC_B = 0x00,
			RLC_C = 0x01,
			RLC_D = 0x02,
			RLC_E = 0x03,
			RLC_H = 0x04,
			RLC_L = 0x05,
			RLC_aHL = 0x06,

			RRC_A = 0x0F,
			RRC_B = 0x08,
			RRC_C = 0x09,
			RRC_D = 0x0A,
			RRC_E = 0x0B,
			RRC_H = 0x0C,
			RRC_L = 0x0D,
			RRC_aHL = 0x0E,

			RL_A = 0x17,
			RL_B = 0x10,
			RL_C = 0x11,
			RL_D = 0x12,
			RL_E = 0x13,
			RL_H = 0x14,
			RL_L = 0x15,
			RL_aHL = 0x16,

			RR_A = 0x1F,
			RR_B = 0x18,
			RR_C = 0x19,
			RR_D = 0x1A,
			RR_E = 0x1B,
			RR_H = 0x1C,
			RR_L = 0x1D,
			RR_aHL = 0x1E,

			SLA_A = 0x27,
			SLA_B = 0x20,
			SLA_C = 0x21,
			SLA_D = 0x22,
			SLA_E = 0x23,
			SLA_H = 0x24,
			SLA_L = 0x25,
			SLA_aHL = 0x26,

			SRA_A = 0x2F,
			SRA_B = 0x28,
			SRA_C = 0x29,
			SRA_D = 0x2A,
			SRA_E = 0x2B,
			SRA_H = 0x2C,
			SRA_L = 0x2D,
			SRA_aHL = 0x2E,

			SRL_A = 0x3F,
			SRL_B = 0x38,
			SRL_C = 0x39,
			SRL_D = 0x3A,
			SRL_E = 0x3B,
			SRL_H = 0x3C,
			SRL_L = 0x3D,
			SRL_aHL = 0x3E,

			SWAP_A = 0x37,
			SWAP_B = 0x30,
			SWAP_C = 0x31,
			SWAP_D = 0x32,
			SWAP_E = 0x33,
			SWAP_H = 0x34,
			SWAP_L = 0x35,
			SWAP_aHL = 0x36,

			BIT_A_0 = 0x47,
			BIT_A_1 = 0x4F,
			BIT_A_2 = 0x57,
			BIT_A_3 = 0x5F,
			BIT_A_4 = 0x67,
			BIT_A_5 = 0x6F,
			BIT_A_6 = 0x77,
			BIT_A_7 = 0x7F,

			BIT_B_0 = 0x40,
			BIT_B_1 = 0x48,
			BIT_B_2 = 0x50,
			BIT_B_3 = 0x58,
			BIT_B_4 = 0x60,
			BIT_B_5 = 0x68,
			BIT_B_6 = 0x70,
			BIT_B_7 = 0x78,

			BIT_C_0 = 0x41,
			BIT_C_1 = 0x49,
			BIT_C_2 = 0x51,
			BIT_C_3 = 0x59,
			BIT_C_4 = 0x61,
			BIT_C_5 = 0x69,
			BIT_C_6 = 0x71,
			BIT_C_7 = 0x79,

			BIT_D_0 = 0x42,
			BIT_D_1 = 0x4A,
			BIT_D_2 = 0x52,
			BIT_D_3 = 0x5A,
			BIT_D_4 = 0x62,
			BIT_D_5 = 0x6A,
			BIT_D_6 = 0x72,
			BIT_D_7 = 0x7A,

			BIT_E_0 = 0x43,
			BIT_E_1 = 0x4B,
			BIT_E_2 = 0x53,
			BIT_E_3 = 0x5B,
			BIT_E_4 = 0x63,
			BIT_E_5 = 0x6B,
			BIT_E_6 = 0x73,
			BIT_E_7 = 0x7B,

			BIT_H_0 = 0x44,
			BIT_H_1 = 0x4C,
			BIT_H_2 = 0x54,
			BIT_H_3 = 0x5C,
			BIT_H_4 = 0x64,
			BIT_H_5 = 0x6C,
			BIT_H_6 = 0x74,
			BIT_H_7 = 0x7C,

			BIT_L_0 = 0x45,
			BIT_L_1 = 0x4D,
			BIT_L_2 = 0x55,
			BIT_L_3 = 0x5D,
			BIT_L_4 = 0x65,
			BIT_L_5 = 0x6D,
			BIT_L_6 = 0x75,
			BIT_L_7 = 0x7D,

			BIT_aHL_0 = 0x46,
			BIT_aHL_1 = 0x4E,
			BIT_aHL_2 = 0x56,
			BIT_aHL_3 = 0x5E,
			BIT_aHL_4 = 0x66,
			BIT_aHL_5 = 0x6E,
			BIT_aHL_6 = 0x76,
			BIT_aHL_7 = 0x7E,

			RES_A_0 = 0x87,
			RES_A_1 = 0x8F,
			RES_A_2 = 0x97,
			RES_A_3 = 0x9F,
			RES_A_4 = 0xA7,
			RES_A_5 = 0xAF,
			RES_A_6 = 0xB7,
			RES_A_7 = 0xBF,

			RES_B_0 = 0x80,
			RES_B_1 = 0x88,
			RES_B_2 = 0x90,
			RES_B_3 = 0x98,
			RES_B_4 = 0xA0,
			RES_B_5 = 0xA8,
			RES_B_6 = 0xB0,
			RES_B_7 = 0xB8,

			RES_C_0 = 0x81,
			RES_C_1 = 0x89,
			RES_C_2 = 0x91,
			RES_C_3 = 0x99,
			RES_C_4 = 0xA1,
			RES_C_5 = 0xA9,
			RES_C_6 = 0xB1,
			RES_C_7 = 0xB9,

			RES_D_0 = 0x82,
			RES_D_1 = 0x8A,
			RES_D_2 = 0x92,
			RES_D_3 = 0x9A,
			RES_D_4 = 0xA2,
			RES_D_5 = 0xAA,
			RES_D_6 = 0xB2,
			RES_D_7 = 0xBA,

			RES_E_0 = 0x83,
			RES_E_1 = 0x8B,
			RES_E_2 = 0x93,
			RES_E_3 = 0x9B,
			RES_E_4 = 0xA3,
			RES_E_5 = 0xAB,
			RES_E_6 = 0xB3,
			RES_E_7 = 0xBB,

			RES_H_0 = 0x84,
			RES_H_1 = 0x8C,
			RES_H_2 = 0x94,
			RES_H_3 = 0x9C,
			RES_H_4 = 0xA4,
			RES_H_5 = 0xAC,
			RES_H_6 = 0xB4,
			RES_H_7 = 0xBC,

			RES_L_0 = 0x85,
			RES_L_1 = 0x8D,
			RES_L_2 = 0x95,
			RES_L_3 = 0x9D,
			RES_L_4 = 0xA5,
			RES_L_5 = 0xAD,
			RES_L_6 = 0xB5,
			RES_L_7 = 0xBD,

			RES_aHL_0 = 0x86,
			RES_aHL_1 = 0x8E,
			RES_aHL_2 = 0x96,
			RES_aHL_3 = 0x9E,
			RES_aHL_4 = 0xA6,
			RES_aHL_5 = 0xAE,
			RES_aHL_6 = 0xB6,
			RES_aHL_7 = 0xBE,

			SET_A_0 = 0xC7,
			SET_A_1 = 0xCF,
			SET_A_2 = 0xD7,
			SET_A_3 = 0xDF,
			SET_A_4 = 0xE7,
			SET_A_5 = 0xEF,
			SET_A_6 = 0xF7,
			SET_A_7 = 0xFF,

			SET_B_0 = 0xC0,
			SET_B_1 = 0xC8,
			SET_B_2 = 0xD0,
			SET_B_3 = 0xD8,
			SET_B_4 = 0xE0,
			SET_B_5 = 0xE8,
			SET_B_6 = 0xF0,
			SET_B_7 = 0xF8,

			SET_C_0 = 0xC1,
			SET_C_1 = 0xC9,
			SET_C_2 = 0xD1,
			SET_C_3 = 0xD9,
			SET_C_4 = 0xE1,
			SET_C_5 = 0xE9,
			SET_C_6 = 0xF1,
			SET_C_7 = 0xF9,

			SET_D_0 = 0xC2,
			SET_D_1 = 0xCA,
			SET_D_2 = 0xD2,
			SET_D_3 = 0xDA,
			SET_D_4 = 0xE2,
			SET_D_5 = 0xEA,
			SET_D_6 = 0xF2,
			SET_D_7 = 0xFA,

			SET_E_0 = 0xC3,
			SET_E_1 = 0xCB,
			SET_E_2 = 0xD3,
			SET_E_3 = 0xDB,
			SET_E_4 = 0xE3,
			SET_E_5 = 0xEB,
			SET_E_6 = 0xF3,
			SET_E_7 = 0xFB,

			SET_H_0 = 0xC4,
			SET_H_1 = 0xCC,
			SET_H_2 = 0xD4,
			SET_H_3 = 0xDC,
			SET_H_4 = 0xE4,
			SET_H_5 = 0xEC,
			SET_H_6 = 0xF4,
			SET_H_7 = 0xFC,

			SET_L_0 = 0xC5,
			SET_L_1 = 0xCD,
			SET_L_2 = 0xD5,
			SET_L_3 = 0xDD,
			SET_L_4 = 0xE5,
			SET_L_5 = 0xED,
			SET_L_6 = 0xF5,
			SET_L_7 = 0xFD,

			SET_aHL_0 = 0xC6,
			SET_aHL_1 = 0xCE,
			SET_aHL_2 = 0xD6,
			SET_aHL_3 = 0xDE,
			SET_aHL_4 = 0xE6,
			SET_aHL_5 = 0xEE,
			SET_aHL_6 = 0xF6,
			SET_aHL_7 = 0xFE,
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

				info[ExtendedInstruction::RLC_A]   = {  8_u16, 1_u16, "RLC A" };
				info[ExtendedInstruction::RLC_B]   = {  8_u16, 1_u16, "RLC B" };
				info[ExtendedInstruction::RLC_C]   = {  8_u16, 1_u16, "RLC C" };
				info[ExtendedInstruction::RLC_D]   = {  8_u16, 1_u16, "RLC D" };
				info[ExtendedInstruction::RLC_E]   = {  8_u16, 1_u16, "RLC E" };
				info[ExtendedInstruction::RLC_H]   = {  8_u16, 1_u16, "RLC H" };
				info[ExtendedInstruction::RLC_L]   = {  8_u16, 1_u16, "RLC L" };
				info[ExtendedInstruction::RLC_aHL] = { 16_u16, 1_u16, "RLC (HL)" };

				info[ExtendedInstruction::RRC_A]   = {  8_u16, 1_u16, "RRC A" };
				info[ExtendedInstruction::RRC_B]   = {  8_u16, 1_u16, "RRC B" };
				info[ExtendedInstruction::RRC_C]   = {  8_u16, 1_u16, "RRC C" };
				info[ExtendedInstruction::RRC_D]   = {  8_u16, 1_u16, "RRC D" };
				info[ExtendedInstruction::RRC_E]   = {  8_u16, 1_u16, "RRC E" };
				info[ExtendedInstruction::RRC_H]   = {  8_u16, 1_u16, "RRC H" };
				info[ExtendedInstruction::RRC_L]   = {  8_u16, 1_u16, "RRC L" };
				info[ExtendedInstruction::RRC_aHL] = { 16_u16, 1_u16, "RRC (HL)" };

				info[ExtendedInstruction::RL_A]   = {  8_u16, 1_u16, "RL A" };
				info[ExtendedInstruction::RL_B]   = {  8_u16, 1_u16, "RL B" };
				info[ExtendedInstruction::RL_C]   = {  8_u16, 1_u16, "RL C" };
				info[ExtendedInstruction::RL_D]   = {  8_u16, 1_u16, "RL D" };
				info[ExtendedInstruction::RL_E]   = {  8_u16, 1_u16, "RL E" };
				info[ExtendedInstruction::RL_H]   = {  8_u16, 1_u16, "RL H" };
				info[ExtendedInstruction::RL_L]   = {  8_u16, 1_u16, "RL L" };
				info[ExtendedInstruction::RL_aHL] = { 16_u16, 1_u16, "RL (HL)" };

				info[ExtendedInstruction::RR_A]   = {  8_u16, 1_u16, "RR A" };
				info[ExtendedInstruction::RR_B]   = {  8_u16, 1_u16, "RR B" };
				info[ExtendedInstruction::RR_C]   = {  8_u16, 1_u16, "RR C" };
				info[ExtendedInstruction::RR_D]   = {  8_u16, 1_u16, "RR D" };
				info[ExtendedInstruction::RR_E]   = {  8_u16, 1_u16, "RR E" };
				info[ExtendedInstruction::RR_H]   = {  8_u16, 1_u16, "RR H" };
				info[ExtendedInstruction::RR_L]   = {  8_u16, 1_u16, "RR L" };
				info[ExtendedInstruction::RR_aHL] = { 16_u16, 1_u16, "RR (HL)" };

				info[ExtendedInstruction::SLA_A]   = {  8_u16, 1_u16, "SLA A" };
				info[ExtendedInstruction::SLA_B]   = {  8_u16, 1_u16, "SLA B" };
				info[ExtendedInstruction::SLA_C]   = {  8_u16, 1_u16, "SLA C" };
				info[ExtendedInstruction::SLA_D]   = {  8_u16, 1_u16, "SLA D" };
				info[ExtendedInstruction::SLA_E]   = {  8_u16, 1_u16, "SLA E" };
				info[ExtendedInstruction::SLA_H]   = {  8_u16, 1_u16, "SLA H" };
				info[ExtendedInstruction::SLA_L]   = {  8_u16, 1_u16, "SLA L" };
				info[ExtendedInstruction::SLA_aHL] = { 16_u16, 1_u16, "SLA (HL)" };

				info[ExtendedInstruction::SRA_A]   = {  8_u16, 1_u16, "SRA A" };
				info[ExtendedInstruction::SRA_B]   = {  8_u16, 1_u16, "SRA B" };
				info[ExtendedInstruction::SRA_C]   = {  8_u16, 1_u16, "SRA C" };
				info[ExtendedInstruction::SRA_D]   = {  8_u16, 1_u16, "SRA D" };
				info[ExtendedInstruction::SRA_E]   = {  8_u16, 1_u16, "SRA E" };
				info[ExtendedInstruction::SRA_H]   = {  8_u16, 1_u16, "SRA H" };
				info[ExtendedInstruction::SRA_L]   = {  8_u16, 1_u16, "SRA L" };
				info[ExtendedInstruction::SRA_aHL] = { 16_u16, 1_u16, "SRA (HL)" };

				info[ExtendedInstruction::SRL_A]   = {  8_u16, 1_u16, "SRL A" };
				info[ExtendedInstruction::SRL_B]   = {  8_u16, 1_u16, "SRL B" };
				info[ExtendedInstruction::SRL_C]   = {  8_u16, 1_u16, "SRL C" };
				info[ExtendedInstruction::SRL_D]   = {  8_u16, 1_u16, "SRL D" };
				info[ExtendedInstruction::SRL_E]   = {  8_u16, 1_u16, "SRL E" };
				info[ExtendedInstruction::SRL_H]   = {  8_u16, 1_u16, "SRL H" };
				info[ExtendedInstruction::SRL_L]   = {  8_u16, 1_u16, "SRL L" };
				info[ExtendedInstruction::SRL_aHL] = { 16_u16, 1_u16, "SRL (HL)" };

				info[ExtendedInstruction::SWAP_A]   = {  8_u16, 1_u16, "SWAP A" };
				info[ExtendedInstruction::SWAP_B]   = {  8_u16, 1_u16, "SWAP B" };
				info[ExtendedInstruction::SWAP_C]   = {  8_u16, 1_u16, "SWAP C" };
				info[ExtendedInstruction::SWAP_D]   = {  8_u16, 1_u16, "SWAP D" };
				info[ExtendedInstruction::SWAP_E]   = {  8_u16, 1_u16, "SWAP E" };
				info[ExtendedInstruction::SWAP_H]   = {  8_u16, 1_u16, "SWAP H" };
				info[ExtendedInstruction::SWAP_L]   = {  8_u16, 1_u16, "SWAP L" };
				info[ExtendedInstruction::SWAP_aHL] = { 16_u16, 1_u16, "SWAP (HL)" };

				info[ExtendedInstruction::BIT_A_0]   = {  8_u16, 1_u16, "BIT A,0" };
				info[ExtendedInstruction::BIT_B_0]   = {  8_u16, 1_u16, "BIT B,0" };
				info[ExtendedInstruction::BIT_C_0]   = {  8_u16, 1_u16, "BIT C,0" };
				info[ExtendedInstruction::BIT_D_0]   = {  8_u16, 1_u16, "BIT D,0" };
				info[ExtendedInstruction::BIT_E_0]   = {  8_u16, 1_u16, "BIT E,0" };
				info[ExtendedInstruction::BIT_H_0]   = {  8_u16, 1_u16, "BIT H,0" };
				info[ExtendedInstruction::BIT_L_0]   = {  8_u16, 1_u16, "BIT L,0" };
				info[ExtendedInstruction::BIT_aHL_0] = { 16_u16, 1_u16, "BIT (HL),0" };

				info[ExtendedInstruction::BIT_A_1]   = {  8_u16, 1_u16, "BIT A,1" };
				info[ExtendedInstruction::BIT_B_1]   = {  8_u16, 1_u16, "BIT B,1" };
				info[ExtendedInstruction::BIT_C_1]   = {  8_u16, 1_u16, "BIT C,1" };
				info[ExtendedInstruction::BIT_D_1]   = {  8_u16, 1_u16, "BIT D,1" };
				info[ExtendedInstruction::BIT_E_1]   = {  8_u16, 1_u16, "BIT E,1" };
				info[ExtendedInstruction::BIT_H_1]   = {  8_u16, 1_u16, "BIT H,1" };
				info[ExtendedInstruction::BIT_L_1]   = {  8_u16, 1_u16, "BIT L,1" };
				info[ExtendedInstruction::BIT_aHL_1] = { 16_u16, 1_u16, "BIT (HL),1" };

				info[ExtendedInstruction::BIT_A_2]   = {  8_u16, 1_u16, "BIT A,2" };
				info[ExtendedInstruction::BIT_B_2]   = {  8_u16, 1_u16, "BIT B,2" };
				info[ExtendedInstruction::BIT_C_2]   = {  8_u16, 1_u16, "BIT C,2" };
				info[ExtendedInstruction::BIT_D_2]   = {  8_u16, 1_u16, "BIT D,2" };
				info[ExtendedInstruction::BIT_E_2]   = {  8_u16, 1_u16, "BIT E,2" };
				info[ExtendedInstruction::BIT_H_2]   = {  8_u16, 1_u16, "BIT H,2" };
				info[ExtendedInstruction::BIT_L_2]   = {  8_u16, 1_u16, "BIT L,2" };
				info[ExtendedInstruction::BIT_aHL_2] = { 16_u16, 1_u16, "BIT (HL),2" };

				info[ExtendedInstruction::BIT_A_3]   = {  8_u16, 1_u16, "BIT A,3" };
				info[ExtendedInstruction::BIT_B_3]   = {  8_u16, 1_u16, "BIT B,3" };
				info[ExtendedInstruction::BIT_C_3]   = {  8_u16, 1_u16, "BIT C,3" };
				info[ExtendedInstruction::BIT_D_3]   = {  8_u16, 1_u16, "BIT D,3" };
				info[ExtendedInstruction::BIT_E_3]   = {  8_u16, 1_u16, "BIT E,3" };
				info[ExtendedInstruction::BIT_H_3]   = {  8_u16, 1_u16, "BIT H,3" };
				info[ExtendedInstruction::BIT_L_3]   = {  8_u16, 1_u16, "BIT L,3" };
				info[ExtendedInstruction::BIT_aHL_3] = { 16_u16, 1_u16, "BIT (HL),3" };

				info[ExtendedInstruction::BIT_A_4]   = {  8_u16, 1_u16, "BIT A,4" };
				info[ExtendedInstruction::BIT_B_4]   = {  8_u16, 1_u16, "BIT B,4" };
				info[ExtendedInstruction::BIT_C_4]   = {  8_u16, 1_u16, "BIT C,4" };
				info[ExtendedInstruction::BIT_D_4]   = {  8_u16, 1_u16, "BIT D,4" };
				info[ExtendedInstruction::BIT_E_4]   = {  8_u16, 1_u16, "BIT E,4" };
				info[ExtendedInstruction::BIT_H_4]   = {  8_u16, 1_u16, "BIT H,4" };
				info[ExtendedInstruction::BIT_L_4]   = {  8_u16, 1_u16, "BIT L,4" };
				info[ExtendedInstruction::BIT_aHL_4] = { 16_u16, 1_u16, "BIT (HL),4" };

				info[ExtendedInstruction::BIT_A_5]   = {  8_u16, 1_u16, "BIT A,5" };
				info[ExtendedInstruction::BIT_B_5]   = {  8_u16, 1_u16, "BIT B,5" };
				info[ExtendedInstruction::BIT_C_5]   = {  8_u16, 1_u16, "BIT C,5" };
				info[ExtendedInstruction::BIT_D_5]   = {  8_u16, 1_u16, "BIT D,5" };
				info[ExtendedInstruction::BIT_E_5]   = {  8_u16, 1_u16, "BIT E,5" };
				info[ExtendedInstruction::BIT_H_5]   = {  8_u16, 1_u16, "BIT H,5" };
				info[ExtendedInstruction::BIT_L_5]   = {  8_u16, 1_u16, "BIT L,5" };
				info[ExtendedInstruction::BIT_aHL_5] = { 16_u16, 1_u16, "BIT (HL),5" };

				info[ExtendedInstruction::BIT_A_6]   = {  8_u16, 1_u16, "BIT A,6" };
				info[ExtendedInstruction::BIT_B_6]   = {  8_u16, 1_u16, "BIT B,6" };
				info[ExtendedInstruction::BIT_C_6]   = {  8_u16, 1_u16, "BIT C,6" };
				info[ExtendedInstruction::BIT_D_6]   = {  8_u16, 1_u16, "BIT D,6" };
				info[ExtendedInstruction::BIT_E_6]   = {  8_u16, 1_u16, "BIT E,6" };
				info[ExtendedInstruction::BIT_H_6]   = {  8_u16, 1_u16, "BIT H,6" };
				info[ExtendedInstruction::BIT_L_6]   = {  8_u16, 1_u16, "BIT L,6" };
				info[ExtendedInstruction::BIT_aHL_6] = { 16_u16, 1_u16, "BIT (HL),6" };

				info[ExtendedInstruction::BIT_A_7]   = {  8_u16, 1_u16, "BIT A,7" };
				info[ExtendedInstruction::BIT_B_7]   = {  8_u16, 1_u16, "BIT B,7" };
				info[ExtendedInstruction::BIT_C_7]   = {  8_u16, 1_u16, "BIT C,7" };
				info[ExtendedInstruction::BIT_D_7]   = {  8_u16, 1_u16, "BIT D,7" };
				info[ExtendedInstruction::BIT_E_7]   = {  8_u16, 1_u16, "BIT E,7" };
				info[ExtendedInstruction::BIT_H_7]   = {  8_u16, 1_u16, "BIT H,7" };
				info[ExtendedInstruction::BIT_L_7]   = {  8_u16, 1_u16, "BIT L,7" };
				info[ExtendedInstruction::BIT_aHL_7] = { 16_u16, 1_u16, "BIT (HL),7" };

				info[ExtendedInstruction::BIT_A_0]   = {  8_u16, 1_u16, "BIT A,0" };
				info[ExtendedInstruction::BIT_B_0]   = {  8_u16, 1_u16, "BIT B,0" };
				info[ExtendedInstruction::BIT_C_0]   = {  8_u16, 1_u16, "BIT C,0" };
				info[ExtendedInstruction::BIT_D_0]   = {  8_u16, 1_u16, "BIT D,0" };
				info[ExtendedInstruction::BIT_E_0]   = {  8_u16, 1_u16, "BIT E,0" };
				info[ExtendedInstruction::BIT_H_0]   = {  8_u16, 1_u16, "BIT H,0" };
				info[ExtendedInstruction::BIT_L_0]   = {  8_u16, 1_u16, "BIT L,0" };
				info[ExtendedInstruction::BIT_aHL_0] = { 16_u16, 1_u16, "BIT (HL),0" };

				info[ExtendedInstruction::RES_A_1]   = {  8_u16, 1_u16, "RES A,1" };
				info[ExtendedInstruction::RES_B_1]   = {  8_u16, 1_u16, "RES B,1" };
				info[ExtendedInstruction::RES_C_1]   = {  8_u16, 1_u16, "RES C,1" };
				info[ExtendedInstruction::RES_D_1]   = {  8_u16, 1_u16, "RES D,1" };
				info[ExtendedInstruction::RES_E_1]   = {  8_u16, 1_u16, "RES E,1" };
				info[ExtendedInstruction::RES_H_1]   = {  8_u16, 1_u16, "RES H,1" };
				info[ExtendedInstruction::RES_L_1]   = {  8_u16, 1_u16, "RES L,1" };
				info[ExtendedInstruction::RES_aHL_1] = { 16_u16, 1_u16, "RES (HL),1" };

				info[ExtendedInstruction::RES_A_2]   = {  8_u16, 1_u16, "RES A,2" };
				info[ExtendedInstruction::RES_B_2]   = {  8_u16, 1_u16, "RES B,2" };
				info[ExtendedInstruction::RES_C_2]   = {  8_u16, 1_u16, "RES C,2" };
				info[ExtendedInstruction::RES_D_2]   = {  8_u16, 1_u16, "RES D,2" };
				info[ExtendedInstruction::RES_E_2]   = {  8_u16, 1_u16, "RES E,2" };
				info[ExtendedInstruction::RES_H_2]   = {  8_u16, 1_u16, "RES H,2" };
				info[ExtendedInstruction::RES_L_2]   = {  8_u16, 1_u16, "RES L,2" };
				info[ExtendedInstruction::RES_aHL_2] = { 16_u16, 1_u16, "RES (HL),2" };

				info[ExtendedInstruction::RES_A_3]   = {  8_u16, 1_u16, "RES A,3" };
				info[ExtendedInstruction::RES_B_3]   = {  8_u16, 1_u16, "RES B,3" };
				info[ExtendedInstruction::RES_C_3]   = {  8_u16, 1_u16, "RES C,3" };
				info[ExtendedInstruction::RES_D_3]   = {  8_u16, 1_u16, "RES D,3" };
				info[ExtendedInstruction::RES_E_3]   = {  8_u16, 1_u16, "RES E,3" };
				info[ExtendedInstruction::RES_H_3]   = {  8_u16, 1_u16, "RES H,3" };
				info[ExtendedInstruction::RES_L_3]   = {  8_u16, 1_u16, "RES L,3" };
				info[ExtendedInstruction::RES_aHL_3] = { 16_u16, 1_u16, "RES (HL),3" };

				info[ExtendedInstruction::RES_A_4]   = {  8_u16, 1_u16, "RES A,4" };
				info[ExtendedInstruction::RES_B_4]   = {  8_u16, 1_u16, "RES B,4" };
				info[ExtendedInstruction::RES_C_4]   = {  8_u16, 1_u16, "RES C,4" };
				info[ExtendedInstruction::RES_D_4]   = {  8_u16, 1_u16, "RES D,4" };
				info[ExtendedInstruction::RES_E_4]   = {  8_u16, 1_u16, "RES E,4" };
				info[ExtendedInstruction::RES_H_4]   = {  8_u16, 1_u16, "RES H,4" };
				info[ExtendedInstruction::RES_L_4]   = {  8_u16, 1_u16, "RES L,4" };
				info[ExtendedInstruction::RES_aHL_4] = { 16_u16, 1_u16, "RES (HL),4" };

				info[ExtendedInstruction::RES_A_5]   = {  8_u16, 1_u16, "RES A,5" };
				info[ExtendedInstruction::RES_B_5]   = {  8_u16, 1_u16, "RES B,5" };
				info[ExtendedInstruction::RES_C_5]   = {  8_u16, 1_u16, "RES C,5" };
				info[ExtendedInstruction::RES_D_5]   = {  8_u16, 1_u16, "RES D,5" };
				info[ExtendedInstruction::RES_E_5]   = {  8_u16, 1_u16, "RES E,5" };
				info[ExtendedInstruction::RES_H_5]   = {  8_u16, 1_u16, "RES H,5" };
				info[ExtendedInstruction::RES_L_5]   = {  8_u16, 1_u16, "RES L,5" };
				info[ExtendedInstruction::RES_aHL_5] = { 16_u16, 1_u16, "RES (HL),5" };

				info[ExtendedInstruction::RES_A_6]   = {  8_u16, 1_u16, "RES A,6" };
				info[ExtendedInstruction::RES_B_6]   = {  8_u16, 1_u16, "RES B,6" };
				info[ExtendedInstruction::RES_C_6]   = {  8_u16, 1_u16, "RES C,6" };
				info[ExtendedInstruction::RES_D_6]   = {  8_u16, 1_u16, "RES D,6" };
				info[ExtendedInstruction::RES_E_6]   = {  8_u16, 1_u16, "RES E,6" };
				info[ExtendedInstruction::RES_H_6]   = {  8_u16, 1_u16, "RES H,6" };
				info[ExtendedInstruction::RES_L_6]   = {  8_u16, 1_u16, "RES L,6" };
				info[ExtendedInstruction::RES_aHL_6] = { 16_u16, 1_u16, "RES (HL),6" };

				info[ExtendedInstruction::RES_A_7]   = {  8_u16, 1_u16, "RES A,7" };
				info[ExtendedInstruction::RES_B_7]   = {  8_u16, 1_u16, "RES B,7" };
				info[ExtendedInstruction::RES_C_7]   = {  8_u16, 1_u16, "RES C,7" };
				info[ExtendedInstruction::RES_D_7]   = {  8_u16, 1_u16, "RES D,7" };
				info[ExtendedInstruction::RES_E_7]   = {  8_u16, 1_u16, "RES E,7" };
				info[ExtendedInstruction::RES_H_7]   = {  8_u16, 1_u16, "RES H,7" };
				info[ExtendedInstruction::RES_L_7]   = {  8_u16, 1_u16, "RES L,7" };
				info[ExtendedInstruction::RES_aHL_7] = { 16_u16, 1_u16, "RES (HL),7" };

				info[ExtendedInstruction::SET_A_0]   = {  8_u16, 1_u16, "SET A,0" };
				info[ExtendedInstruction::SET_B_0]   = {  8_u16, 1_u16, "SET B,0" };
				info[ExtendedInstruction::SET_C_0]   = {  8_u16, 1_u16, "SET C,0" };
				info[ExtendedInstruction::SET_D_0]   = {  8_u16, 1_u16, "SET D,0" };
				info[ExtendedInstruction::SET_E_0]   = {  8_u16, 1_u16, "SET E,0" };
				info[ExtendedInstruction::SET_H_0]   = {  8_u16, 1_u16, "SET H,0" };
				info[ExtendedInstruction::SET_L_0]   = {  8_u16, 1_u16, "SET L,0" };
				info[ExtendedInstruction::SET_aHL_0] = { 16_u16, 1_u16, "SET (HL),0" };

				info[ExtendedInstruction::SET_A_1]   = {  8_u16, 1_u16, "SET A,1" };
				info[ExtendedInstruction::SET_B_1]   = {  8_u16, 1_u16, "SET B,1" };
				info[ExtendedInstruction::SET_C_1]   = {  8_u16, 1_u16, "SET C,1" };
				info[ExtendedInstruction::SET_D_1]   = {  8_u16, 1_u16, "SET D,1" };
				info[ExtendedInstruction::SET_E_1]   = {  8_u16, 1_u16, "SET E,1" };
				info[ExtendedInstruction::SET_H_1]   = {  8_u16, 1_u16, "SET H,1" };
				info[ExtendedInstruction::SET_L_1]   = {  8_u16, 1_u16, "SET L,1" };
				info[ExtendedInstruction::SET_aHL_1] = { 16_u16, 1_u16, "SET (HL),1" };

				info[ExtendedInstruction::SET_A_2]   = {  8_u16, 1_u16, "SET A,2" };
				info[ExtendedInstruction::SET_B_2]   = {  8_u16, 1_u16, "SET B,2" };
				info[ExtendedInstruction::SET_C_2]   = {  8_u16, 1_u16, "SET C,2" };
				info[ExtendedInstruction::SET_D_2]   = {  8_u16, 1_u16, "SET D,2" };
				info[ExtendedInstruction::SET_E_2]   = {  8_u16, 1_u16, "SET E,2" };
				info[ExtendedInstruction::SET_H_2]   = {  8_u16, 1_u16, "SET H,2" };
				info[ExtendedInstruction::SET_L_2]   = {  8_u16, 1_u16, "SET L,2" };
				info[ExtendedInstruction::SET_aHL_2] = { 16_u16, 1_u16, "SET (HL),2" };

				info[ExtendedInstruction::SET_A_3]   = {  8_u16, 1_u16, "SET A,3" };
				info[ExtendedInstruction::SET_B_3]   = {  8_u16, 1_u16, "SET B,3" };
				info[ExtendedInstruction::SET_C_3]   = {  8_u16, 1_u16, "SET C,3" };
				info[ExtendedInstruction::SET_D_3]   = {  8_u16, 1_u16, "SET D,3" };
				info[ExtendedInstruction::SET_E_3]   = {  8_u16, 1_u16, "SET E,3" };
				info[ExtendedInstruction::SET_H_3]   = {  8_u16, 1_u16, "SET H,3" };
				info[ExtendedInstruction::SET_L_3]   = {  8_u16, 1_u16, "SET L,3" };
				info[ExtendedInstruction::SET_aHL_3] = { 16_u16, 1_u16, "SET (HL),3" };

				info[ExtendedInstruction::SET_A_4]   = {  8_u16, 1_u16, "SET A,4" };
				info[ExtendedInstruction::SET_B_4]   = {  8_u16, 1_u16, "SET B,4" };
				info[ExtendedInstruction::SET_C_4]   = {  8_u16, 1_u16, "SET C,4" };
				info[ExtendedInstruction::SET_D_4]   = {  8_u16, 1_u16, "SET D,4" };
				info[ExtendedInstruction::SET_E_4]   = {  8_u16, 1_u16, "SET E,4" };
				info[ExtendedInstruction::SET_H_4]   = {  8_u16, 1_u16, "SET H,4" };
				info[ExtendedInstruction::SET_L_4]   = {  8_u16, 1_u16, "SET L,4" };
				info[ExtendedInstruction::SET_aHL_4] = { 16_u16, 1_u16, "SET (HL),4" };

				info[ExtendedInstruction::SET_A_5]   = {  8_u16, 1_u16, "SET A,5" };
				info[ExtendedInstruction::SET_B_5]   = {  8_u16, 1_u16, "SET B,5" };
				info[ExtendedInstruction::SET_C_5]   = {  8_u16, 1_u16, "SET C,5" };
				info[ExtendedInstruction::SET_D_5]   = {  8_u16, 1_u16, "SET D,5" };
				info[ExtendedInstruction::SET_E_5]   = {  8_u16, 1_u16, "SET E,5" };
				info[ExtendedInstruction::SET_H_5]   = {  8_u16, 1_u16, "SET H,5" };
				info[ExtendedInstruction::SET_L_5]   = {  8_u16, 1_u16, "SET L,5" };
				info[ExtendedInstruction::SET_aHL_5] = { 16_u16, 1_u16, "SET (HL),5" };

				info[ExtendedInstruction::SET_A_6]   = {  8_u16, 1_u16, "SET A,6" };
				info[ExtendedInstruction::SET_B_6]   = {  8_u16, 1_u16, "SET B,6" };
				info[ExtendedInstruction::SET_C_6]   = {  8_u16, 1_u16, "SET C,6" };
				info[ExtendedInstruction::SET_D_6]   = {  8_u16, 1_u16, "SET D,6" };
				info[ExtendedInstruction::SET_E_6]   = {  8_u16, 1_u16, "SET E,6" };
				info[ExtendedInstruction::SET_H_6]   = {  8_u16, 1_u16, "SET H,6" };
				info[ExtendedInstruction::SET_L_6]   = {  8_u16, 1_u16, "SET L,6" };
				info[ExtendedInstruction::SET_aHL_6] = { 16_u16, 1_u16, "SET (HL),6" };

				info[ExtendedInstruction::SET_A_7]   = {  8_u16, 1_u16, "SET A,7" };
				info[ExtendedInstruction::SET_B_7]   = {  8_u16, 1_u16, "SET B,7" };
				info[ExtendedInstruction::SET_C_7]   = {  8_u16, 1_u16, "SET C,7" };
				info[ExtendedInstruction::SET_D_7]   = {  8_u16, 1_u16, "SET D,7" };
				info[ExtendedInstruction::SET_E_7]   = {  8_u16, 1_u16, "SET E,7" };
				info[ExtendedInstruction::SET_H_7]   = {  8_u16, 1_u16, "SET H,7" };
				info[ExtendedInstruction::SET_L_7]   = {  8_u16, 1_u16, "SET L,7" };
				info[ExtendedInstruction::SET_aHL_7] = { 16_u16, 1_u16, "SET (HL),7" };

				return info;
			}();
		}

		constexpr std::optional<uint16_t> GetCycles(ExtendedInstruction::Enum a_Instruction)
		{
			return Internal::g_InstructionInfo[a_Instruction].m_Cycles;
		}

		constexpr std::optional<uint16_t> GetSize(ExtendedInstruction::Enum a_Instruction)
		{
			return Internal::g_InstructionInfo[a_Instruction].m_Size;
		}

		constexpr std::optional<std::string_view> ToString(ExtendedInstruction::Enum a_Instruction)
		{
			return Internal::g_InstructionInfo[a_Instruction].m_Name;
		}
	}
}

#endif