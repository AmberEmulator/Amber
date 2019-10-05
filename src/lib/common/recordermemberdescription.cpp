#include <common/recordermemberdescription.hpp>

using namespace Amber;
using namespace Common;

std::string_view RecorderMemberDescription::GetName() const noexcept
{
	return m_Name;
}

size_t RecorderMemberDescription::GetSize() const noexcept
{
	switch (m_Type)
	{
		case RecorderMemberType::uint8:
		return 1;

		case RecorderMemberType::uint16:
		return 1;

		case RecorderMemberType::uint32:
		return 1;

		case RecorderMemberType::uint64:
		return 1;

		case RecorderMemberType::Raw:
		return m_Size;

		default:
		return 0;
	}
}

RecorderMemberType::Enum RecorderMemberDescription::GetType() const noexcept
{
	return m_Type;
}

void RecorderMemberDescription::SetName(std::string_view a_Name) noexcept
{
	m_Name = a_Name;
}

void RecorderMemberDescription::SetSize(size_t a_Size) noexcept
{
	m_Size = a_Size;
}

void RecorderMemberDescription::SetType(RecorderMemberType::Enum a_Type) noexcept
{
	m_Type = a_Type;
}