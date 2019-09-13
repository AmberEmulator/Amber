#ifndef H_AMBER_GAMEBOY_BASICCARTRIDGE
#define H_AMBER_GAMEBOY_BASICCARTRIDGE

#include <gameboy/api.hpp>
#include <gameboy/cartridge.hpp>

#include <common/ram.hpp>

#include <memory>

namespace Amber::Gameboy
{
	class GAMEBOY_API BasicCartridge : public Cartridge
	{
		public:
		BasicCartridge(size_t a_ROMSize, size_t a_RAMSize);

		const CartridgeHeader& GetHeader() const override;
		Common::ROM16<false>& GetROM() noexcept;
		Common::RAM16<false>& GetRAM() noexcept;

		uint8_t Load8(Address a_Address) const override;
		void Store8(Address a_Address, uint8_t a_Value) override;

		protected:
		Common::ROM16<false> m_ROM;
		Common::RAM16<false> m_RAM;
	};
}

#endif