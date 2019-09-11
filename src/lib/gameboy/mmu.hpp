#ifndef H_AMBER_GAMEBOY_MMU
#define H_AMBER_GAMEBOY_MMU

#include <gameboy/api.hpp>

#include <common/ram.hpp>

namespace Amber::Gameboy
{
	class CPU;
	class Joypad;
	class PPU;

	class GAMEBOY_API MMU : public Common::MemoryHelper<uint16_t, false>
	{
		public:
		using Memory = Common::Memory16;
		using ROM = Common::ROM16<false>;
		using RAM = Common::RAM16<false>;

		void SetBootROM(ROM* a_BootROM);
		void SetROM(ROM* a_Rom);
		void SetVRAM(RAM* a_VRAM);
		void SetWRAM(RAM* a_WRAM);
		void SetCPU(CPU* a_CPU);
		void SetPPU(PPU* a_PPU);
		void SetJoypad(Joypad* a_Joypad);

		uint8_t Load8(Address a_Address) const override;
		void Store8(Address a_Address, uint8_t a_Value) override;

		void Reset();

		private:
		ROM* m_BootROM = nullptr;
		ROM* m_ROM = nullptr;
		RAM* m_VRAM = nullptr;
		RAM* m_WRAM = nullptr;
		CPU* m_CPU = nullptr;
		PPU* m_PPU = nullptr;
		Joypad* m_Joypad = nullptr;
		uint8_t m_OAM[160] = {};
		uint8_t m_HRAM[127] = {};
		bool m_BootROMEnabled = true;

		uint8_t m_TestCharacter;
	};
}


#endif