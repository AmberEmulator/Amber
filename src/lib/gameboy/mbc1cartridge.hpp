#ifndef H_AMBER_GAMEBOY_MBC1CARTRIDGE
#define H_AMBER_GAMEBOY_MBC1CARTRIDGE

#include <gameboy/api.hpp>
#include <gameboy/basiccartridge.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API MBC1Cartridge : public BasicCartridge
	{
		public:
		MBC1Cartridge(size_t a_ROMSize, size_t a_RAMSize);
		
		uint8_t Load8(Address a_Address) const override;
		void Store8(Address a_Address, uint8_t a_Value) override;

		private:
		bool m_ROMBanking = true;
		bool m_RAMEnabled = false;
		uint8_t m_ROMBank = 0x01;
		uint8_t m_RAMBank = 0x00;
	};
}

#endif