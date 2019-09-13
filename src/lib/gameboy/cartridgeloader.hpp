#ifndef H_AMBER_GAMEBOY_CARTRIDGELOADER
#define H_AMBER_GAMEBOY_CARTRIDGELOADER

#include <gameboy/api.hpp>
#include <gameboy/basiccartridge.hpp>

#include <iosfwd>
#include <memory>

namespace Amber::Gameboy
{
	class GAMEBOY_API CartridgeLoader
	{
		public:
		std::unique_ptr<BasicCartridge> LoadCartridge(std::istream& a_Source) const;
		std::unique_ptr<BasicCartridge> CreateCartridge(const CartridgeHeader& a_Header) const;
	};
}

#endif