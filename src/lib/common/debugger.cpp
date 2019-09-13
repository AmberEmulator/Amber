#include <common/debugger.hpp>

#include <limits>

using namespace Amber;
using namespace Common;

Debugger::~Debugger() noexcept = default;

uint64_t Debugger::GetMaximumAddress() const noexcept
{
	return std::numeric_limits<uint64_t>::max();
}

size_t Debugger::GetMaximumAddressWidth() const noexcept
{
	// TODO: portable
	const uint64_t maximum_address = GetMaximumAddress();
	unsigned long result;
	_BitScanReverse64(&result, maximum_address);
	return static_cast<size_t>((result + 4) / 4);
}

std::string Debugger::GetAddressName(uint64_t a_Address) const
{
	// Hex character lookup table
	constexpr char hex_characters[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	// Calculate address width
	const size_t address_Width = GetMaximumAddressWidth();

	// Create string
	std::string name;
	name.resize(address_Width);

	// Find all characters by shifting and masking the address
	for (size_t i = 0; i < address_Width; ++i)
	{
		name[(address_Width - 1) - i] = hex_characters[(a_Address >> (i * 4)) & 0xF];
	}

	return name;
}

bool Debugger::HasBreakpoint(uint64_t a_Address) const noexcept
{
	return false;
}

void Debugger::SetBreakpoint(uint64_t a_Address, bool a_Enabled)
{
}

std::unordered_set<uint64_t> Debugger::GetBreakpoints() const
{
	return {};
}

bool Debugger::Microstep()
{
	Step();
	return true;
}