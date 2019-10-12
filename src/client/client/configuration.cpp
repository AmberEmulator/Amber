#include <client/configuration.hpp>

#include <cstdlib>
#include <filesystem>

using namespace Amber;
using namespace Client;

void Configuration::Save(const std::filesystem::path& a_Path)
{
}

void Configuration::Load(const std::filesystem::path& a_Path)
{
}

size_t Configuration::GetRecentROMCount() const noexcept
{
	return m_RecentROMs.size();
}

const std::filesystem::path& Configuration::GetRecentROM(size_t a_Index) const noexcept
{
	return m_RecentROMs[a_Index];
}

void Configuration::PushRecentROM(const std::filesystem::path& a_ROM)
{
	for (auto it = m_RecentROMs.begin(); it != m_RecentROMs.end(); ++it)
	{
		if (*it == a_ROM)
		{
			m_RecentROMs.erase(it);
			break;
		}
	}

	m_RecentROMs.push_front(a_ROM);
	if (m_RecentROMs.size() > 10)
	{
		m_RecentROMs.pop_back();
	}
}