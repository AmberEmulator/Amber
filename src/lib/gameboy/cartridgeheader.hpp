#ifndef H_AMBER_GAMEBOY_CARTRIDGEHEADER
#define H_AMBER_GAMEBOY_CARTRIDGEHEADER

#include <gameboy/api.hpp>
#include <gameboy/cartridgetype.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API CartridgeHeader
	{
		public:
		static constexpr uint16_t EntryPointOffset     = 0x0000;
		static constexpr uint16_t LogoOffset           = 0x0004;
		static constexpr uint16_t TitleOffset          = 0x0034;
		static constexpr uint16_t ManufacturerOffset   = 0x003F;
		static constexpr uint16_t CGBFlagOffset        = 0x0043;
		static constexpr uint16_t NewLicenseeOffset    = 0x0044;
		static constexpr uint16_t SGBFlagOffset        = 0x0046;
		static constexpr uint16_t CartridgeTypeOffset  = 0x0047;
		static constexpr uint16_t ROMSizeOffset        = 0x0048;
		static constexpr uint16_t RAMSizeOffset        = 0x0049;
		static constexpr uint16_t DestinationOffset    = 0x004A;
		static constexpr uint16_t OldLicenseeOffset    = 0x004B;
		static constexpr uint16_t VersionOffset        = 0x004C;
		static constexpr uint16_t HeaderChecksumOffset = 0x004D;
		static constexpr uint16_t GlobalChecksumOffset = 0x004E;

		static constexpr uint16_t HeaderAddress         = 0x0100;
		static constexpr uint16_t EntryPointAddress     = HeaderAddress + EntryPointOffset;
		static constexpr uint16_t LogoAddress           = HeaderAddress + LogoOffset;
		static constexpr uint16_t TitleAddress          = HeaderAddress + TitleOffset;
		static constexpr uint16_t ManufacturerAddress   = HeaderAddress + ManufacturerOffset;
		static constexpr uint16_t CGBFlagAddress        = HeaderAddress + CGBFlagOffset;
		static constexpr uint16_t NewLicenseeAddress    = HeaderAddress + NewLicenseeOffset;
		static constexpr uint16_t SGBFlagAddress        = HeaderAddress + SGBFlagOffset;
		static constexpr uint16_t CartridgeTypeAddress  = HeaderAddress + CartridgeTypeOffset;
		static constexpr uint16_t ROMSizeAddress        = HeaderAddress + ROMSizeOffset;
		static constexpr uint16_t RAMSizeAddress        = HeaderAddress + RAMSizeOffset;
		static constexpr uint16_t DestinationAddress    = HeaderAddress + DestinationOffset;
		static constexpr uint16_t OldLicenseeAddress    = HeaderAddress + OldLicenseeOffset;
		static constexpr uint16_t VersionAddress        = HeaderAddress + VersionOffset;
		static constexpr uint16_t HeaderChecksumAddress = HeaderAddress + HeaderChecksumOffset;
		static constexpr uint16_t GlobalChecksumAddress = HeaderAddress + GlobalChecksumOffset;

		static constexpr uint16_t HeaderSize = 0x50;
		static constexpr uint16_t EntryPointSize     = LogoAddress - EntryPointAddress;
		static constexpr uint16_t LogoSize           = TitleAddress - LogoAddress;
		static constexpr uint16_t TitleSize          = ManufacturerAddress - TitleAddress; // May be larger on older cartridges
		static constexpr uint16_t ManufacturerSize   = CGBFlagAddress - ManufacturerAddress;
		static constexpr uint16_t CGBFlagSize        = NewLicenseeAddress - CGBFlagAddress;
		static constexpr uint16_t NewLicenseeSize    = SGBFlagAddress - NewLicenseeAddress;
		static constexpr uint16_t SGBFlagSize        = CartridgeTypeAddress - SGBFlagAddress;
		static constexpr uint16_t CartridgeTypeSize  = ROMSizeAddress - CartridgeTypeAddress;
		static constexpr uint16_t ROMSizeSize        = RAMSizeAddress - ROMSizeAddress;
		static constexpr uint16_t RAMSizeSize        = DestinationAddress - RAMSizeAddress;
		static constexpr uint16_t DestinationSize    = OldLicenseeAddress - DestinationAddress;
		static constexpr uint16_t OldLicenseeSize    = VersionAddress - OldLicenseeAddress;
		static constexpr uint16_t VersionSize        = HeaderChecksumAddress - VersionAddress;
		static constexpr uint16_t HeaderChecksumSize = GlobalChecksumAddress - HeaderChecksumAddress;
		static constexpr uint16_t GlobalChecksumSize = (HeaderAddress + HeaderSize) - GlobalChecksumAddress;

		std::string_view GetTitle() const noexcept;
		CartridgeType::Enum GetCartridgeType() const noexcept;
		size_t GetROMSize() const noexcept;
		size_t GetRAMSize() const noexcept;

		private:
		uint8_t m_Data[HeaderSize];
	};
}

#endif