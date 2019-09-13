#ifndef H_AMBER_GAMEBOY_CARTRIDGE
#define H_AMBER_GAMEBOY_CARTRIDGE

#include <gameboy/api.hpp>
#include <gameboy/cartridgeheader.hpp>

#include <common/memory.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API Cartridge : public Common::MemoryHelper<uint16_t, false>
	{
		public:
		static constexpr size_t ROMBankSize = 0x4000;
		static constexpr size_t RAMBankSize = 0x2000;

		virtual ~Cartridge() noexcept = 0;

		virtual const CartridgeHeader& GetHeader() const = 0;
	};
}

#endif