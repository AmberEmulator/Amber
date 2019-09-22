#ifndef H_AMBER_GAMEBOY_MMU
#define H_AMBER_GAMEBOY_MMU

#include <gameboy/api.hpp>

#include <common/memory.hpp>

namespace Amber::Gameboy
{
	class CPU;
	class Joypad;
	class PPU;

	class GAMEBOY_API MMU : public Common::MemoryHelper<uint16_t, false>
	{
		public:
		MMU();

		void SetBootROM(Memory* a_BootROM);
		void SetCartridge(Memory* a_Cartridge);
		void SetVRAM(Memory* a_VRAM);
		void SetWRAM(Memory* a_WRAM);
		void SetCPU(CPU* a_CPU);
		void SetPPU(PPU* a_PPU);
		void SetJoypad(Joypad* a_Joypad);

		uint8_t Load8(Address a_Address) const override;
		void Store8(Address a_Address, uint8_t a_Value) override;

		void Reset();

		private:
		using LoadOp = uint8_t(MMU::*)(uint16_t a_Address) const;
		using StoreOp = void (MMU::*)(uint16_t a_Address, uint8_t a_Value);

		uint8_t LoadNOP(uint16_t a_Address) const;
		uint8_t LoadBoot(uint16_t a_Address) const;
		uint8_t LoadLastPage(uint16_t a_Address) const;
		template <auto Member, uint16_t a_Offset>
		uint8_t LoadMemory(uint16_t a_Address) const
		{
			return (this->*Member)->Load8(a_Address - a_Offset);
		}
		template <auto Member, uint16_t a_Offset>
		uint8_t LoadArray(uint16_t a_Address) const
		{
			return (this->*Member)[a_Address - a_Offset];
		}
		template <auto Member, auto Op>
		uint8_t LoadRegister(uint16_t a_Address) const
		{
			return ((this->*Member)->*Op)();
		}

		void StoreNOP(uint16_t a_Address, uint8_t a_Value);
		void StoreBoot(uint16_t a_Address, uint8_t a_Value);
		void StoreDisableBoot(uint16_t a_Address, uint8_t a_Value);
		void StoreLastPage(uint16_t a_Address, uint8_t a_Value);
		template <auto Member, uint16_t a_Offset>
		void StoreMemory(uint16_t a_Address, uint8_t a_Value)
		{
			(this->*Member)->Store(a_Address - a_Offset, a_Value);
		}
		template <auto Member, uint16_t a_Offset>
		void StoreArray(uint16_t a_Address, uint8_t a_Value)
		{
			(this->*Member)[a_Address - a_Offset] = a_Value;
		}
		template <auto Member, auto Op>
		void StoreRegister(uint16_t a_Address, uint8_t a_Value)
		{
			((this->*Member)->*Op)(a_Value);
		}

		LoadOp m_PageLoads[16];
		StoreOp m_PageStores[16];
		LoadOp m_LastLoads[512];
		StoreOp m_LastStores[512];

		Memory* m_BootROM = nullptr;
		Memory* m_Cartridge = nullptr;
		Memory* m_VRAM = nullptr;
		Memory* m_WRAM = nullptr;
		CPU* m_CPU = nullptr;
		PPU* m_PPU = nullptr;
		Joypad* m_Joypad = nullptr;
		uint8_t m_OAM[160] = {};
		uint8_t m_HRAM[127] = {};
	};
}


#endif