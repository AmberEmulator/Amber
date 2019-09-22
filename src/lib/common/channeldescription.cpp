#include <common/channeldescription.hpp>

using namespace Amber;
using namespace Common;

std::string_view ChannelDescription::GetName() const noexcept
{
	return m_Name;
}

size_t ChannelDescription::GetSize() const noexcept
{
	return m_Size;
}

void ChannelDescription::SetName(std::string_view a_Name)
{
	m_Name = a_Name;
}

void ChannelDescription::SetSize(size_t a_Size) noexcept
{
	m_Size = a_Size;
}