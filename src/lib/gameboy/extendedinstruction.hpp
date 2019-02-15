#ifndef H_DEMU_GAMEBOY_EXTENDEDINSTRUCTION
#define H_DEMU_GAMEBOY_EXTENDEDINSTRUCTION

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

		constexpr uint16_t GetCycleCount(ExtendedInstruction::Enum a_Instruction)
		{
			switch (a_Instruction)
			{
				case ExtendedInstruction::RLC_A:
				case ExtendedInstruction::RLC_B:
				case ExtendedInstruction::RLC_C:
				case ExtendedInstruction::RLC_D:
				case ExtendedInstruction::RLC_E:
				case ExtendedInstruction::RLC_H:
				case ExtendedInstruction::RLC_L:
				case ExtendedInstruction::RRC_A:
				case ExtendedInstruction::RRC_B:
				case ExtendedInstruction::RRC_C:
				case ExtendedInstruction::RRC_D:
				case ExtendedInstruction::RRC_E:
				case ExtendedInstruction::RRC_H:
				case ExtendedInstruction::RRC_L:
				case ExtendedInstruction::RL_A:
				case ExtendedInstruction::RL_B:
				case ExtendedInstruction::RL_C:
				case ExtendedInstruction::RL_D:
				case ExtendedInstruction::RL_E:
				case ExtendedInstruction::RL_H:
				case ExtendedInstruction::RL_L:
				case ExtendedInstruction::RR_A:
				case ExtendedInstruction::RR_B:
				case ExtendedInstruction::RR_C:
				case ExtendedInstruction::RR_D:
				case ExtendedInstruction::RR_E:
				case ExtendedInstruction::RR_H:
				case ExtendedInstruction::RR_L:
				case ExtendedInstruction::SWAP_A:
				case ExtendedInstruction::SWAP_B:
				case ExtendedInstruction::SWAP_C:
				case ExtendedInstruction::SWAP_D:
				case ExtendedInstruction::SWAP_E:
				case ExtendedInstruction::SWAP_H:
				case ExtendedInstruction::SWAP_L:
				case ExtendedInstruction::BIT_A_0:
				case ExtendedInstruction::BIT_A_1:
				case ExtendedInstruction::BIT_A_2:
				case ExtendedInstruction::BIT_A_3:
				case ExtendedInstruction::BIT_A_4:
				case ExtendedInstruction::BIT_A_5:
				case ExtendedInstruction::BIT_A_6:
				case ExtendedInstruction::BIT_A_7:
				case ExtendedInstruction::BIT_B_0:
				case ExtendedInstruction::BIT_B_1:
				case ExtendedInstruction::BIT_B_2:
				case ExtendedInstruction::BIT_B_3:
				case ExtendedInstruction::BIT_B_4:
				case ExtendedInstruction::BIT_B_5:
				case ExtendedInstruction::BIT_B_6:
				case ExtendedInstruction::BIT_B_7:
				case ExtendedInstruction::BIT_C_0:
				case ExtendedInstruction::BIT_C_1:
				case ExtendedInstruction::BIT_C_2:
				case ExtendedInstruction::BIT_C_3:
				case ExtendedInstruction::BIT_C_4:
				case ExtendedInstruction::BIT_C_5:
				case ExtendedInstruction::BIT_C_6:
				case ExtendedInstruction::BIT_C_7:
				case ExtendedInstruction::BIT_D_0:
				case ExtendedInstruction::BIT_D_1:
				case ExtendedInstruction::BIT_D_2:
				case ExtendedInstruction::BIT_D_3:
				case ExtendedInstruction::BIT_D_4:
				case ExtendedInstruction::BIT_D_5:
				case ExtendedInstruction::BIT_D_6:
				case ExtendedInstruction::BIT_D_7:
				case ExtendedInstruction::BIT_E_0:
				case ExtendedInstruction::BIT_E_1:
				case ExtendedInstruction::BIT_E_2:
				case ExtendedInstruction::BIT_E_3:
				case ExtendedInstruction::BIT_E_4:
				case ExtendedInstruction::BIT_E_5:
				case ExtendedInstruction::BIT_E_6:
				case ExtendedInstruction::BIT_E_7:
				case ExtendedInstruction::BIT_H_0:
				case ExtendedInstruction::BIT_H_1:
				case ExtendedInstruction::BIT_H_2:
				case ExtendedInstruction::BIT_H_3:
				case ExtendedInstruction::BIT_H_4:
				case ExtendedInstruction::BIT_H_5:
				case ExtendedInstruction::BIT_H_6:
				case ExtendedInstruction::BIT_H_7:
				case ExtendedInstruction::BIT_L_0:
				case ExtendedInstruction::BIT_L_1:
				case ExtendedInstruction::BIT_L_2:
				case ExtendedInstruction::BIT_L_3:
				case ExtendedInstruction::BIT_L_4:
				case ExtendedInstruction::BIT_L_5:
				case ExtendedInstruction::BIT_L_6:
				case ExtendedInstruction::BIT_L_7:
				case ExtendedInstruction::RES_A_0:
				case ExtendedInstruction::RES_A_1:
				case ExtendedInstruction::RES_A_2:
				case ExtendedInstruction::RES_A_3:
				case ExtendedInstruction::RES_A_4:
				case ExtendedInstruction::RES_A_5:
				case ExtendedInstruction::RES_A_6:
				case ExtendedInstruction::RES_A_7:
				case ExtendedInstruction::RES_B_0:
				case ExtendedInstruction::RES_B_1:
				case ExtendedInstruction::RES_B_2:
				case ExtendedInstruction::RES_B_3:
				case ExtendedInstruction::RES_B_4:
				case ExtendedInstruction::RES_B_5:
				case ExtendedInstruction::RES_B_6:
				case ExtendedInstruction::RES_B_7:
				case ExtendedInstruction::RES_C_0:
				case ExtendedInstruction::RES_C_1:
				case ExtendedInstruction::RES_C_2:
				case ExtendedInstruction::RES_C_3:
				case ExtendedInstruction::RES_C_4:
				case ExtendedInstruction::RES_C_5:
				case ExtendedInstruction::RES_C_6:
				case ExtendedInstruction::RES_C_7:
				case ExtendedInstruction::RES_D_0:
				case ExtendedInstruction::RES_D_1:
				case ExtendedInstruction::RES_D_2:
				case ExtendedInstruction::RES_D_3:
				case ExtendedInstruction::RES_D_4:
				case ExtendedInstruction::RES_D_5:
				case ExtendedInstruction::RES_D_6:
				case ExtendedInstruction::RES_D_7:
				case ExtendedInstruction::RES_E_0:
				case ExtendedInstruction::RES_E_1:
				case ExtendedInstruction::RES_E_2:
				case ExtendedInstruction::RES_E_3:
				case ExtendedInstruction::RES_E_4:
				case ExtendedInstruction::RES_E_5:
				case ExtendedInstruction::RES_E_6:
				case ExtendedInstruction::RES_E_7:
				case ExtendedInstruction::RES_H_0:
				case ExtendedInstruction::RES_H_1:
				case ExtendedInstruction::RES_H_2:
				case ExtendedInstruction::RES_H_3:
				case ExtendedInstruction::RES_H_4:
				case ExtendedInstruction::RES_H_5:
				case ExtendedInstruction::RES_H_6:
				case ExtendedInstruction::RES_H_7:
				case ExtendedInstruction::RES_L_0:
				case ExtendedInstruction::RES_L_1:
				case ExtendedInstruction::RES_L_2:
				case ExtendedInstruction::RES_L_3:
				case ExtendedInstruction::RES_L_4:
				case ExtendedInstruction::RES_L_5:
				case ExtendedInstruction::RES_L_6:
				case ExtendedInstruction::RES_L_7:
				case ExtendedInstruction::SET_A_0:
				case ExtendedInstruction::SET_A_1:
				case ExtendedInstruction::SET_A_2:
				case ExtendedInstruction::SET_A_3:
				case ExtendedInstruction::SET_A_4:
				case ExtendedInstruction::SET_A_5:
				case ExtendedInstruction::SET_A_6:
				case ExtendedInstruction::SET_A_7:
				case ExtendedInstruction::SET_B_0:
				case ExtendedInstruction::SET_B_1:
				case ExtendedInstruction::SET_B_2:
				case ExtendedInstruction::SET_B_3:
				case ExtendedInstruction::SET_B_4:
				case ExtendedInstruction::SET_B_5:
				case ExtendedInstruction::SET_B_6:
				case ExtendedInstruction::SET_B_7:
				case ExtendedInstruction::SET_C_0:
				case ExtendedInstruction::SET_C_1:
				case ExtendedInstruction::SET_C_2:
				case ExtendedInstruction::SET_C_3:
				case ExtendedInstruction::SET_C_4:
				case ExtendedInstruction::SET_C_5:
				case ExtendedInstruction::SET_C_6:
				case ExtendedInstruction::SET_C_7:
				case ExtendedInstruction::SET_D_0:
				case ExtendedInstruction::SET_D_1:
				case ExtendedInstruction::SET_D_2:
				case ExtendedInstruction::SET_D_3:
				case ExtendedInstruction::SET_D_4:
				case ExtendedInstruction::SET_D_5:
				case ExtendedInstruction::SET_D_6:
				case ExtendedInstruction::SET_D_7:
				case ExtendedInstruction::SET_E_0:
				case ExtendedInstruction::SET_E_1:
				case ExtendedInstruction::SET_E_2:
				case ExtendedInstruction::SET_E_3:
				case ExtendedInstruction::SET_E_4:
				case ExtendedInstruction::SET_E_5:
				case ExtendedInstruction::SET_E_6:
				case ExtendedInstruction::SET_E_7:
				case ExtendedInstruction::SET_H_0:
				case ExtendedInstruction::SET_H_1:
				case ExtendedInstruction::SET_H_2:
				case ExtendedInstruction::SET_H_3:
				case ExtendedInstruction::SET_H_4:
				case ExtendedInstruction::SET_H_5:
				case ExtendedInstruction::SET_H_6:
				case ExtendedInstruction::SET_H_7:
				case ExtendedInstruction::SET_L_0:
				case ExtendedInstruction::SET_L_1:
				case ExtendedInstruction::SET_L_2:
				case ExtendedInstruction::SET_L_3:
				case ExtendedInstruction::SET_L_4:
				case ExtendedInstruction::SET_L_5:
				case ExtendedInstruction::SET_L_6:
				case ExtendedInstruction::SET_L_7:
				return 8;

				case ExtendedInstruction::RLC_aHL:
				case ExtendedInstruction::RRC_aHL:
				case ExtendedInstruction::RL_aHL:
				case ExtendedInstruction::RR_aHL:
				case ExtendedInstruction::SWAP_aHL:
				case ExtendedInstruction::BIT_aHL_0:
				case ExtendedInstruction::BIT_aHL_1:
				case ExtendedInstruction::BIT_aHL_2:
				case ExtendedInstruction::BIT_aHL_3:
				case ExtendedInstruction::BIT_aHL_4:
				case ExtendedInstruction::BIT_aHL_5:
				case ExtendedInstruction::BIT_aHL_6:
				case ExtendedInstruction::BIT_aHL_7:
				case ExtendedInstruction::RES_aHL_0:
				case ExtendedInstruction::RES_aHL_1:
				case ExtendedInstruction::RES_aHL_2:
				case ExtendedInstruction::RES_aHL_3:
				case ExtendedInstruction::RES_aHL_4:
				case ExtendedInstruction::RES_aHL_5:
				case ExtendedInstruction::RES_aHL_6:
				case ExtendedInstruction::RES_aHL_7:
				case ExtendedInstruction::SET_aHL_0:
				case ExtendedInstruction::SET_aHL_1:
				case ExtendedInstruction::SET_aHL_2:
				case ExtendedInstruction::SET_aHL_3:
				case ExtendedInstruction::SET_aHL_4:
				case ExtendedInstruction::SET_aHL_5:
				case ExtendedInstruction::SET_aHL_6:
				case ExtendedInstruction::SET_aHL_7:
				return 16;

				default:
				return 0;
			}
		}

		constexpr std::optional<std::string_view> ToString(ExtendedInstruction::Enum a_Instruction)
		{
			switch (a_Instruction)
			{
				case ExtendedInstruction::RLC_A:     return "RLC A";
				case ExtendedInstruction::RLC_B:     return "RLC B";
				case ExtendedInstruction::RLC_C:     return "RLC C";
				case ExtendedInstruction::RLC_D:     return "RLC D";
				case ExtendedInstruction::RLC_E:     return "RLC E";
				case ExtendedInstruction::RLC_H:     return "RLC H";
				case ExtendedInstruction::RLC_L:     return "RLC L";
				case ExtendedInstruction::RLC_aHL:   return "RLC (HL)";

				case ExtendedInstruction::RRC_A:     return "RRC A";
				case ExtendedInstruction::RRC_B:     return "RRC B";
				case ExtendedInstruction::RRC_C:     return "RRC C";
				case ExtendedInstruction::RRC_D:     return "RRC D";
				case ExtendedInstruction::RRC_E:     return "RRC E";
				case ExtendedInstruction::RRC_H:     return "RRC H";
				case ExtendedInstruction::RRC_L:     return "RRC L";
				case ExtendedInstruction::RRC_aHL:   return "RRC (HL)";

				case ExtendedInstruction::RL_A:      return "RL A";
				case ExtendedInstruction::RL_B:      return "RL B";
				case ExtendedInstruction::RL_C:      return "RL C";
				case ExtendedInstruction::RL_D:      return "RL D";
				case ExtendedInstruction::RL_E:      return "RL E";
				case ExtendedInstruction::RL_H:      return "RL H";
				case ExtendedInstruction::RL_L:      return "RL L";
				case ExtendedInstruction::RL_aHL:    return "RL (HL)";

				case ExtendedInstruction::RR_A:      return "RR A";
				case ExtendedInstruction::RR_B:      return "RR B";
				case ExtendedInstruction::RR_C:      return "RR C";
				case ExtendedInstruction::RR_D:      return "RR D";
				case ExtendedInstruction::RR_E:      return "RR E";
				case ExtendedInstruction::RR_H:      return "RR H";
				case ExtendedInstruction::RR_L:      return "RR L";
				case ExtendedInstruction::RR_aHL:    return "RR (HL)";

				case ExtendedInstruction::SWAP_A:    return "SWAP A";
				case ExtendedInstruction::SWAP_B:    return "SWAP B";
				case ExtendedInstruction::SWAP_C:    return "SWAP C";
				case ExtendedInstruction::SWAP_D:    return "SWAP D";
				case ExtendedInstruction::SWAP_E:    return "SWAP E";
				case ExtendedInstruction::SWAP_H:    return "SWAP H";
				case ExtendedInstruction::SWAP_L:    return "SWAP L";
				case ExtendedInstruction::SWAP_aHL:  return "SWAP (HL)";

				case ExtendedInstruction::BIT_A_0:   return "BIT A,0";
				case ExtendedInstruction::BIT_A_1:   return "BIT A,1";
				case ExtendedInstruction::BIT_A_2:   return "BIT A,2";
				case ExtendedInstruction::BIT_A_3:   return "BIT A,3";
				case ExtendedInstruction::BIT_A_4:   return "BIT A,4";
				case ExtendedInstruction::BIT_A_5:   return "BIT A,5";
				case ExtendedInstruction::BIT_A_6:   return "BIT A,6";
				case ExtendedInstruction::BIT_A_7:   return "BIT A,7";

				case ExtendedInstruction::BIT_B_0:   return "BIT B,0";
				case ExtendedInstruction::BIT_B_1:   return "BIT B,1";
				case ExtendedInstruction::BIT_B_2:   return "BIT B,2";
				case ExtendedInstruction::BIT_B_3:   return "BIT B,3";
				case ExtendedInstruction::BIT_B_4:   return "BIT B,4";
				case ExtendedInstruction::BIT_B_5:   return "BIT B,5";
				case ExtendedInstruction::BIT_B_6:   return "BIT B,6";
				case ExtendedInstruction::BIT_B_7:   return "BIT B,7";

				case ExtendedInstruction::BIT_C_0:   return "BIT C,0";
				case ExtendedInstruction::BIT_C_1:   return "BIT C,1";
				case ExtendedInstruction::BIT_C_2:   return "BIT C,2";
				case ExtendedInstruction::BIT_C_3:   return "BIT C,3";
				case ExtendedInstruction::BIT_C_4:   return "BIT C,4";
				case ExtendedInstruction::BIT_C_5:   return "BIT C,5";
				case ExtendedInstruction::BIT_C_6:   return "BIT C,6";
				case ExtendedInstruction::BIT_C_7:   return "BIT C,7";

				case ExtendedInstruction::BIT_D_0:   return "BIT D,0";
				case ExtendedInstruction::BIT_D_1:   return "BIT D,1";
				case ExtendedInstruction::BIT_D_2:   return "BIT D,2";
				case ExtendedInstruction::BIT_D_3:   return "BIT D,3";
				case ExtendedInstruction::BIT_D_4:   return "BIT D,4";
				case ExtendedInstruction::BIT_D_5:   return "BIT D,5";
				case ExtendedInstruction::BIT_D_6:   return "BIT D,6";
				case ExtendedInstruction::BIT_D_7:   return "BIT D,7";

				case ExtendedInstruction::BIT_E_0:   return "BIT E,0";
				case ExtendedInstruction::BIT_E_1:   return "BIT E,1";
				case ExtendedInstruction::BIT_E_2:   return "BIT E,2";
				case ExtendedInstruction::BIT_E_3:   return "BIT E,3";
				case ExtendedInstruction::BIT_E_4:   return "BIT E,4";
				case ExtendedInstruction::BIT_E_5:   return "BIT E,5";
				case ExtendedInstruction::BIT_E_6:   return "BIT E,6";
				case ExtendedInstruction::BIT_E_7:   return "BIT E,7";

				case ExtendedInstruction::BIT_H_0:   return "BIT H,0";
				case ExtendedInstruction::BIT_H_1:   return "BIT H,1";
				case ExtendedInstruction::BIT_H_2:   return "BIT H,2";
				case ExtendedInstruction::BIT_H_3:   return "BIT H,3";
				case ExtendedInstruction::BIT_H_4:   return "BIT H,4";
				case ExtendedInstruction::BIT_H_5:   return "BIT H,5";
				case ExtendedInstruction::BIT_H_6:   return "BIT H,6";
				case ExtendedInstruction::BIT_H_7:   return "BIT H,7";

				case ExtendedInstruction::BIT_L_0:   return "BIT L,0";
				case ExtendedInstruction::BIT_L_1:   return "BIT L,1";
				case ExtendedInstruction::BIT_L_2:   return "BIT L,2";
				case ExtendedInstruction::BIT_L_3:   return "BIT L,3";
				case ExtendedInstruction::BIT_L_4:   return "BIT L,4";
				case ExtendedInstruction::BIT_L_5:   return "BIT L,5";
				case ExtendedInstruction::BIT_L_6:   return "BIT L,6";
				case ExtendedInstruction::BIT_L_7:   return "BIT L,7";

				case ExtendedInstruction::BIT_aHL_0: return "BIT (HL),0";
				case ExtendedInstruction::BIT_aHL_1: return "BIT (HL),1";
				case ExtendedInstruction::BIT_aHL_2: return "BIT (HL),2";
				case ExtendedInstruction::BIT_aHL_3: return "BIT (HL),3";
				case ExtendedInstruction::BIT_aHL_4: return "BIT (HL),4";
				case ExtendedInstruction::BIT_aHL_5: return "BIT (HL),5";
				case ExtendedInstruction::BIT_aHL_6: return "BIT (HL),6";
				case ExtendedInstruction::BIT_aHL_7: return "BIT (HL),7";

				case ExtendedInstruction::RES_A_0:   return "RES A,0";
				case ExtendedInstruction::RES_A_1:   return "RES A,1";
				case ExtendedInstruction::RES_A_2:   return "RES A,2";
				case ExtendedInstruction::RES_A_3:   return "RES A,3";
				case ExtendedInstruction::RES_A_4:   return "RES A,4";
				case ExtendedInstruction::RES_A_5:   return "RES A,5";
				case ExtendedInstruction::RES_A_6:   return "RES A,6";
				case ExtendedInstruction::RES_A_7:   return "RES A,7";

				case ExtendedInstruction::RES_B_0:   return "RES B,0";
				case ExtendedInstruction::RES_B_1:   return "RES B,1";
				case ExtendedInstruction::RES_B_2:   return "RES B,2";
				case ExtendedInstruction::RES_B_3:   return "RES B,3";
				case ExtendedInstruction::RES_B_4:   return "RES B,4";
				case ExtendedInstruction::RES_B_5:   return "RES B,5";
				case ExtendedInstruction::RES_B_6:   return "RES B,6";
				case ExtendedInstruction::RES_B_7:   return "RES B,7";

				case ExtendedInstruction::RES_C_0:   return "RES C,0";
				case ExtendedInstruction::RES_C_1:   return "RES C,1";
				case ExtendedInstruction::RES_C_2:   return "RES C,2";
				case ExtendedInstruction::RES_C_3:   return "RES C,3";
				case ExtendedInstruction::RES_C_4:   return "RES C,4";
				case ExtendedInstruction::RES_C_5:   return "RES C,5";
				case ExtendedInstruction::RES_C_6:   return "RES C,6";
				case ExtendedInstruction::RES_C_7:   return "RES C,7";

				case ExtendedInstruction::RES_D_0:   return "RES D,0";
				case ExtendedInstruction::RES_D_1:   return "RES D,1";
				case ExtendedInstruction::RES_D_2:   return "RES D,2";
				case ExtendedInstruction::RES_D_3:   return "RES D,3";
				case ExtendedInstruction::RES_D_4:   return "RES D,4";
				case ExtendedInstruction::RES_D_5:   return "RES D,5";
				case ExtendedInstruction::RES_D_6:   return "RES D,6";
				case ExtendedInstruction::RES_D_7:   return "RES D,7";

				case ExtendedInstruction::RES_E_0:   return "RES E,0";
				case ExtendedInstruction::RES_E_1:   return "RES E,1";
				case ExtendedInstruction::RES_E_2:   return "RES E,2";
				case ExtendedInstruction::RES_E_3:   return "RES E,3";
				case ExtendedInstruction::RES_E_4:   return "RES E,4";
				case ExtendedInstruction::RES_E_5:   return "RES E,5";
				case ExtendedInstruction::RES_E_6:   return "RES E,6";
				case ExtendedInstruction::RES_E_7:   return "RES E,7";

				case ExtendedInstruction::RES_H_0:   return "RES H,0";
				case ExtendedInstruction::RES_H_1:   return "RES H,1";
				case ExtendedInstruction::RES_H_2:   return "RES H,2";
				case ExtendedInstruction::RES_H_3:   return "RES H,3";
				case ExtendedInstruction::RES_H_4:   return "RES H,4";
				case ExtendedInstruction::RES_H_5:   return "RES H,5";
				case ExtendedInstruction::RES_H_6:   return "RES H,6";
				case ExtendedInstruction::RES_H_7:   return "RES H,7";

				case ExtendedInstruction::RES_L_0:   return "RES L,0";
				case ExtendedInstruction::RES_L_1:   return "RES L,1";
				case ExtendedInstruction::RES_L_2:   return "RES L,2";
				case ExtendedInstruction::RES_L_3:   return "RES L,3";
				case ExtendedInstruction::RES_L_4:   return "RES L,4";
				case ExtendedInstruction::RES_L_5:   return "RES L,5";
				case ExtendedInstruction::RES_L_6:   return "RES L,6";
				case ExtendedInstruction::RES_L_7:   return "RES L,7";

				case ExtendedInstruction::RES_aHL_0: return "RES (HL),0";
				case ExtendedInstruction::RES_aHL_1: return "RES (HL),1";
				case ExtendedInstruction::RES_aHL_2: return "RES (HL),2";
				case ExtendedInstruction::RES_aHL_3: return "RES (HL),3";
				case ExtendedInstruction::RES_aHL_4: return "RES (HL),4";
				case ExtendedInstruction::RES_aHL_5: return "RES (HL),5";
				case ExtendedInstruction::RES_aHL_6: return "RES (HL),6";
				case ExtendedInstruction::RES_aHL_7: return "RES (HL),7";

				case ExtendedInstruction::SET_A_0:   return "SET A,0";
				case ExtendedInstruction::SET_A_1:   return "SET A,1";
				case ExtendedInstruction::SET_A_2:   return "SET A,2";
				case ExtendedInstruction::SET_A_3:   return "SET A,3";
				case ExtendedInstruction::SET_A_4:   return "SET A,4";
				case ExtendedInstruction::SET_A_5:   return "SET A,5";
				case ExtendedInstruction::SET_A_6:   return "SET A,6";
				case ExtendedInstruction::SET_A_7:   return "SET A,7";

				case ExtendedInstruction::SET_B_0:   return "SET B,0";
				case ExtendedInstruction::SET_B_1:   return "SET B,1";
				case ExtendedInstruction::SET_B_2:   return "SET B,2";
				case ExtendedInstruction::SET_B_3:   return "SET B,3";
				case ExtendedInstruction::SET_B_4:   return "SET B,4";
				case ExtendedInstruction::SET_B_5:   return "SET B,5";
				case ExtendedInstruction::SET_B_6:   return "SET B,6";
				case ExtendedInstruction::SET_B_7:   return "SET B,7";

				case ExtendedInstruction::SET_C_0:   return "SET C,0";
				case ExtendedInstruction::SET_C_1:   return "SET C,1";
				case ExtendedInstruction::SET_C_2:   return "SET C,2";
				case ExtendedInstruction::SET_C_3:   return "SET C,3";
				case ExtendedInstruction::SET_C_4:   return "SET C,4";
				case ExtendedInstruction::SET_C_5:   return "SET C,5";
				case ExtendedInstruction::SET_C_6:   return "SET C,6";
				case ExtendedInstruction::SET_C_7:   return "SET C,7";

				case ExtendedInstruction::SET_D_0:   return "SET D,0";
				case ExtendedInstruction::SET_D_1:   return "SET D,1";
				case ExtendedInstruction::SET_D_2:   return "SET D,2";
				case ExtendedInstruction::SET_D_3:   return "SET D,3";
				case ExtendedInstruction::SET_D_4:   return "SET D,4";
				case ExtendedInstruction::SET_D_5:   return "SET D,5";
				case ExtendedInstruction::SET_D_6:   return "SET D,6";
				case ExtendedInstruction::SET_D_7:   return "SET D,7";

				case ExtendedInstruction::SET_E_0:   return "SET E,0";
				case ExtendedInstruction::SET_E_1:   return "SET E,1";
				case ExtendedInstruction::SET_E_2:   return "SET E,2";
				case ExtendedInstruction::SET_E_3:   return "SET E,3";
				case ExtendedInstruction::SET_E_4:   return "SET E,4";
				case ExtendedInstruction::SET_E_5:   return "SET E,5";
				case ExtendedInstruction::SET_E_6:   return "SET E,6";
				case ExtendedInstruction::SET_E_7:   return "SET E,7";

				case ExtendedInstruction::SET_H_0:   return "SET H,0";
				case ExtendedInstruction::SET_H_1:   return "SET H,1";
				case ExtendedInstruction::SET_H_2:   return "SET H,2";
				case ExtendedInstruction::SET_H_3:   return "SET H,3";
				case ExtendedInstruction::SET_H_4:   return "SET H,4";
				case ExtendedInstruction::SET_H_5:   return "SET H,5";
				case ExtendedInstruction::SET_H_6:   return "SET H,6";
				case ExtendedInstruction::SET_H_7:   return "SET H,7";

				case ExtendedInstruction::SET_L_0:   return "SET L,0";
				case ExtendedInstruction::SET_L_1:   return "SET L,1";
				case ExtendedInstruction::SET_L_2:   return "SET L,2";
				case ExtendedInstruction::SET_L_3:   return "SET L,3";
				case ExtendedInstruction::SET_L_4:   return "SET L,4";
				case ExtendedInstruction::SET_L_5:   return "SET L,5";
				case ExtendedInstruction::SET_L_6:   return "SET L,6";
				case ExtendedInstruction::SET_L_7:   return "SET L,7";

				case ExtendedInstruction::SET_aHL_0: return "SET (HL),0";
				case ExtendedInstruction::SET_aHL_1: return "SET (HL),1";
				case ExtendedInstruction::SET_aHL_2: return "SET (HL),2";
				case ExtendedInstruction::SET_aHL_3: return "SET (HL),3";
				case ExtendedInstruction::SET_aHL_4: return "SET (HL),4";
				case ExtendedInstruction::SET_aHL_5: return "SET (HL),5";
				case ExtendedInstruction::SET_aHL_6: return "SET (HL),6";
				case ExtendedInstruction::SET_aHL_7: return "SET (HL),7";

				default:
				return {};
			}
		}
	}
}

#endif