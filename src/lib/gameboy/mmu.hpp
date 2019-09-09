#ifndef H_AMBER_GAMEBOY_MMU
#define H_AMBER_GAMEBOY_MMU

#include <gameboy/api.hpp>

#include <common/ram.hpp>

namespace Amber::Gameboy
{
	class PPU;

	class GAMEBOY_API MMU : public Common::MemoryHelper<uint16_t, false>
	{
		public:
		using Memory = Common::Memory16;
		using ROM = Common::ROM16<false>;
		using RAM = Common::RAM16<false>;

		MMU();

		void SetBootROM(ROM* a_BootROM);
		void SetROM(ROM* a_Rom);
		void SetVRAM(RAM* a_VRAM);
		void SetWRAM(RAM* a_WRAM);
		void SetPPU(PPU* a_PPU);

		uint8_t Load8(Address a_Address) const override;
		void Store8(Address a_Address, uint8_t a_Value) override;

		private:
		ROM* m_BootROM = nullptr;
		ROM* m_ROM = nullptr;
		RAM* m_VRAM = nullptr;
		RAM* m_WRAM = nullptr;
		PPU* m_PPU = nullptr;
		uint8_t m_HRAM[127];
		bool m_BootROMEnabled = true;
	};
}


#endif