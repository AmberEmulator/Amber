#ifndef H_DEMU_GAMEBOY_REGISTERS
#define H_DEMU_GAMEBOY_REGISTERS

#include <gameboy/api.hpp>

namespace Demu::Gameboy
{
	class Registers
	{
		public:
		static constexpr uint8_t RegisterIndexAF = 0;
		static constexpr uint8_t RegisterIndexBC = 1;
		static constexpr uint8_t RegisterIndexDE = 2;
		static constexpr uint8_t RegisterIndexHL = 3;
		static constexpr uint8_t RegisterIndexSP = 4;
		static constexpr uint8_t RegisterIndexPC = 5;

		static constexpr uint8_t RegisterIndexA = (RegisterIndexAF << 1) + 0;
		static constexpr uint8_t RegisterIndexF = (RegisterIndexAF << 1) + 1;
		static constexpr uint8_t RegisterIndexB = (RegisterIndexBC << 1) + 0;
		static constexpr uint8_t RegisterIndexC = (RegisterIndexBC << 1) + 1;
		static constexpr uint8_t RegisterIndexD = (RegisterIndexDE << 1) + 0;
		static constexpr uint8_t RegisterIndexE = (RegisterIndexDE << 1) + 1;
		static constexpr uint8_t RegisterIndexH = (RegisterIndexHL << 1) + 0;
		static constexpr uint8_t RegisterIndexL = (RegisterIndexHL << 1) + 1;

		static constexpr uint8_t FlagIndexZero = 7;
		static constexpr uint8_t FlagIndexSubtract = 6;
		static constexpr uint8_t FlagIndexHalfCarry = 5;
		static constexpr uint8_t FlagIndexCarry = 4;

		constexpr uint16_t GetRegister16(uint16_t a_Index) const noexcept
		{
			return m_Registers[a_Index];
		}

		constexpr uint16_t GetAF() const noexcept
		{
			return GetRegister16(RegisterIndexAF);
		}

		constexpr uint16_t GetBC() const noexcept
		{
			return GetRegister16(RegisterIndexBC);
		}

		constexpr uint16_t GetDE() const noexcept
		{
			return GetRegister16(RegisterIndexDE);
		}

		constexpr uint16_t GetHL() const noexcept
		{
			return GetRegister16(RegisterIndexHL);
		}

		constexpr uint16_t GetSP() const noexcept
		{
			return GetRegister16(RegisterIndexSP);
		}

		constexpr uint16_t GetPC() const noexcept
		{
			return GetRegister16(RegisterIndexPC);
		}

		constexpr uint8_t GetRegister8(uint8_t a_Index) const noexcept
		{
			const uint8_t shift = (1 - (a_Index & 1)) << 3;
			const uint16_t mask = 0xFFU << shift;

			return static_cast<uint8_t>((GetRegister16(a_Index >> 1) & mask) >> shift);
		}

		constexpr uint8_t GetA() const noexcept
		{
			return GetRegister8(RegisterIndexA);
		}

		constexpr uint8_t GetB() const noexcept
		{
			return GetRegister8(RegisterIndexB);
		}

		constexpr uint8_t GetC() const noexcept
		{
			return GetRegister8(RegisterIndexC);
		}

		constexpr uint8_t GetD() const noexcept
		{
			return GetRegister8(RegisterIndexD);
		}

		constexpr uint8_t GetE() const noexcept
		{
			return GetRegister8(RegisterIndexE);
		}

		constexpr uint8_t GetF() const noexcept
		{
			return GetRegister8(RegisterIndexF);
		}

		constexpr uint8_t GetH() const noexcept
		{
			return GetRegister8(RegisterIndexH);
		}

		constexpr uint8_t GetL() const noexcept
		{
			return GetRegister8(RegisterIndexL);
		}

		constexpr bool GetFlag(uint8_t a_Index) const noexcept
		{
			return (GetF() & (1 << a_Index)) != 0;
		}

		constexpr bool GetZero() const noexcept
		{
			return GetFlag(FlagIndexZero);
		}

		constexpr bool GetSubtract() const noexcept
		{
			return GetFlag(FlagIndexSubtract);
		}

		constexpr bool GetHalfCarry() const noexcept
		{
			return GetFlag(FlagIndexHalfCarry);
		}

		constexpr bool GetCarry() const noexcept
		{
			return GetFlag(FlagIndexCarry);
		}

		constexpr void SetRegister16(uint8_t a_Index, uint16_t a_Value) noexcept
		{
			m_Registers[a_Index] = a_Value;
		}

		constexpr void SetAF(uint16_t a_Value) noexcept
		{
			SetRegister16(RegisterIndexAF, a_Value);
		}

		constexpr void SetBC(uint16_t a_Value) noexcept
		{
			SetRegister16(RegisterIndexBC, a_Value);
		}

		constexpr void SetDE(uint16_t a_Value) noexcept
		{
			SetRegister16(RegisterIndexDE, a_Value);
		}

		constexpr void SetHL(uint16_t a_Value) noexcept
		{
			SetRegister16(RegisterIndexHL, a_Value);
		}

		constexpr void SetSP(uint16_t a_Value) noexcept
		{
			SetRegister16(RegisterIndexSP, a_Value);
		}

		constexpr void SetPC(uint16_t a_Value) noexcept
		{
			SetRegister16(RegisterIndexPC, a_Value);
		}

		constexpr void SetRegister8(uint8_t a_Index, uint8_t a_Value) noexcept
		{
			const uint8_t other = GetRegister8(a_Index ^ 1);
			const uint16_t shift = (1 - (a_Index & 1)) << 3;

			SetRegister16(a_Index >> 1, (static_cast<uint16_t>(a_Value) << shift) | (static_cast<uint16_t>(other) << (8 - shift)));
		}

		constexpr void SetA(uint8_t a_Value) noexcept
		{
			SetRegister8(RegisterIndexA, a_Value);
		}

		constexpr void SetB(uint8_t a_Value) noexcept
		{
			SetRegister8(RegisterIndexB, a_Value);
		}

		constexpr void SetC(uint8_t a_Value) noexcept
		{
			SetRegister8(RegisterIndexC, a_Value);
		}

		constexpr void SetD(uint8_t a_Value) noexcept
		{
			SetRegister8(RegisterIndexD, a_Value);
		}

		constexpr void SetE(uint8_t a_Value) noexcept
		{
			SetRegister8(RegisterIndexE, a_Value);
		}

		constexpr void SetF(uint8_t a_Value) noexcept
		{
			SetRegister8(RegisterIndexF, a_Value);
		}

		constexpr void SetH(uint8_t a_Value) noexcept
		{
			SetRegister8(RegisterIndexH, a_Value);
		}

		constexpr void SetL(uint8_t a_Value) noexcept
		{
			SetRegister8(RegisterIndexL, a_Value);
		}

		constexpr void SetFlag(uint8_t a_Index, bool a_Value) noexcept
		{
			const uint8_t f = GetF();

			if (a_Value)
			{
				SetF(f | (1 << a_Index));
			}
			else
			{
				SetF(f & (~(1 << a_Index)));
			}
		}

		constexpr void SetZero(bool a_Value) noexcept
		{
			SetFlag(FlagIndexZero, a_Value);
		}

		constexpr void SetSubtract(bool a_Value) noexcept
		{
			SetFlag(FlagIndexSubtract, a_Value);
		}

		constexpr void SetHalfCarry(bool a_Value) noexcept
		{
			SetFlag(FlagIndexHalfCarry, a_Value);
		}

		constexpr void SetCarry(bool a_Value) noexcept
		{
			SetFlag(FlagIndexCarry, a_Value);
		}

		private:
		uint16_t m_Registers[6] = {};
	};

	constexpr bool operator == (const Registers& a_Left, const Registers& a_Right) noexcept
	{
		for (uint8_t i = 0; i < 6; ++i)
		{
			if (a_Left.GetRegister16(i) != a_Right.GetRegister16(i))
			{
				return false;
			}
		}

		return true;
	}

	constexpr bool operator != (const Registers& a_Left, const Registers& a_Right) noexcept
	{
		return !(a_Left == a_Right);
	}
}

#endif