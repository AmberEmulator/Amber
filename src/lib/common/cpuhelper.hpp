#ifndef H_AMBER_COMMON_CPUHELPER
#define H_AMBER_COMMON_CPUHELPER

#include <common/api.hpp>

namespace Amber::Common
{
	template <typename CPU>
	class CPUHelper
	{
		public:
		using MicroOp = void (CPU::*)();

		// Composition ops
		using UnaryOp8  = uint8_t  (CPU::*)(uint8_t  a_Value);
		using UnaryOp16 = uint16_t (CPU::*)(uint16_t a_Value);
		using UnaryOp32 = uint32_t (CPU::*)(uint32_t a_Value);
		using UnaryOp64 = uint64_t (CPU::*)(uint64_t a_Value);
		using BinaryOp8  = uint8_t  (CPU::*)(uint8_t  a_Left, uint8_t  a_Right);
		using BinaryOp16 = uint16_t (CPU::*)(uint16_t a_Left, uint16_t a_Right);
		using BinaryOp32 = uint32_t (CPU::*)(uint32_t a_Left, uint32_t a_Right);
		using BinaryOp64 = uint64_t (CPU::*)(uint64_t a_Left, uint64_t a_Right);

	};

}

#endif