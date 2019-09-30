#ifndef H_AMBER_GAMEBOY_MBC2CARTRIDGE
#define H_AMBER_GAMEBOY_MBC2CARTRIDGE

#include <gameboy/api.hpp>
#include <gameboy/basiccartridge.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API MBC2Cartridge : public BasicCartridge
	{
		public:
		MBC2Cartridge(size_t a_ROMSize);

		uint8_t Load8(Address a_Address) const override;
		void Store8(Address a_Address, uint8_t a_Value) override;

		private:
		bool m_RAMEnabled = false;
		uint8_t m_ROMBank = 0x01;
		uint8_t m_RAM[256] = {};
	};
}

#endif