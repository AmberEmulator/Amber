#include <gameboy/device.hpp>

#include <gameboy/cpu.hpp>
#include <gameboy/mmu.hpp>
#include <gameboy/ppu.hpp>

using namespace Amber;
using namespace Gameboy;

Device::Device(const DeviceDescription& a_Description):
	m_Description(a_Description)
{
	m_PPU = std::make_unique<PPU>();
	m_MMU = std::make_unique<MMU>();
	m_CPU = std::make_unique<CPU>(*m_MMU);

	m_PPU->SetCPU(m_CPU.get());

	m_MMU->SetCPU(m_CPU.get());
	m_MMU->SetPPU(m_PPU.get());
}

Device::~Device() = default;

const DeviceDescription& Device::GetDescription() const noexcept
{
	return m_Description;
}

CPU& Device::GetCPU() noexcept
{
	return *m_CPU;
}

PPU& Device::GetPPU() noexcept
{
	return *m_PPU;
}

MMU& Device::GetMMU() noexcept
{
	return *m_MMU;
}

bool Device::Tick()
{
	m_PPU->Tick();
	const bool done = m_CPU->Tick();

	return done;
}

void Device::Reset()
{
	m_CPU->Reset();
	m_PPU->Reset();
	m_MMU->Reset();
}