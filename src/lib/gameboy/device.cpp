#include <gameboy/device.hpp>

#include <gameboy/cpu.hpp>
#include <gameboy/joypad.hpp>
#include <gameboy/mmu.hpp>
#include <gameboy/ppu.hpp>

using namespace Amber;
using namespace Gameboy;

Device::Device(const DeviceDescription& a_Description):
	m_Description(a_Description)
{
	m_MMU = std::make_unique<MMU>();
	m_CPU = std::make_unique<CPU>(*m_MMU);
	m_PPU = std::make_unique<PPU>(*m_MMU);
	m_Joypad = std::make_unique<Joypad>();
	m_VRAM = std::make_unique<RAM>(0x2000);
	m_WRAM = std::make_unique<RAM>(0x2000);

	m_MMU->SetCPU(m_CPU.get());
	m_MMU->SetJoypad(m_Joypad.get());
	m_MMU->SetPPU(m_PPU.get());
	m_MMU->SetVRAM(m_VRAM.get());
	m_MMU->SetWRAM(m_WRAM.get());

	m_PPU->SetCPU(m_CPU.get());

	m_Joypad->SetCPU(m_CPU.get());
}

Device::~Device() = default;

const DeviceDescription& Device::GetDescription() const noexcept
{
	return m_Description;
}

MMU& Device::GetMMU() noexcept
{
	return *m_MMU;
}

CPU& Device::GetCPU() noexcept
{
	return *m_CPU;
}

PPU& Device::GetPPU() noexcept
{
	return *m_PPU;
}

Joypad& Device::GetJoypad() noexcept
{
	return *m_Joypad;
}

bool Device::Tick()
{
	for (size_t i = 0; i < 4; ++i)
	{
		m_PPU->Tick();
	}

	const bool done = m_CPU->Tick();

	return done;
}

void Device::Reset()
{
	m_CPU->Reset();
	m_PPU->Reset();
	m_MMU->Reset();
}