#include <gameboy/cartridgeloader.hpp>

#include <gameboy/cartridge.hpp>
#include <gameboy/cartridgeheader.hpp>

#include <gameboy/basiccartridge.hpp>
#include <gameboy/mbc1cartridge.hpp>
#include <gameboy/mbc2cartridge.hpp>

#include <algorithm>
#include <istream>

using namespace Amber;
using namespace Gameboy;

std::unique_ptr<BasicCartridge> CartridgeLoader::LoadCartridge(std::istream& a_Source) const
{
	CartridgeHeader header;

	a_Source.seekg(CartridgeHeader::HeaderAddress);
	a_Source.read(reinterpret_cast<char*>(&header), sizeof(header));

	auto cartridge = CreateCartridge(header);
	auto& rom = cartridge->GetROM();

	a_Source.seekg(0, std::ios_base::end);
	const size_t rom_size = std::max<size_t>(a_Source.tellg(), rom.GetSize());

	a_Source.seekg(0, std::ios_base::beg);
	a_Source.read(reinterpret_cast<char*>(rom.GetData()), rom_size);

	return cartridge;
}

std::unique_ptr<BasicCartridge> CartridgeLoader::CreateCartridge(const CartridgeHeader& a_Header) const
{
	auto name = a_Header.GetTitle();

	const CartridgeType::Enum cartridge_type = a_Header.GetCartridgeType();
	const size_t rom_size = a_Header.GetROMSize();
	const size_t ram_size = a_Header.GetRAMSize();

	switch (cartridge_type)
	{
		case CartridgeType::ROM:
		return std::make_unique<BasicCartridge>(rom_size, ram_size);
		break;

		case CartridgeType::MBC1:
		case CartridgeType::MBC1_RAM:
		case CartridgeType::MBC1_RAM_BATTERY: // TODO: implement ram battery
		return std::make_unique<MBC1Cartridge>(rom_size, ram_size);
		break;

		case CartridgeType::MBC2_RAM:
		case CartridgeType::MBC2_RAM_BATTERY: // TODO: implement ram battery
		return std::make_unique<MBC2Cartridge>(rom_size);
		break;

		default:
		// TODO: error handling
		return nullptr;
	}
}