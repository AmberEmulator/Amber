#include <common/recorderchanneldescription.hpp>

#include <common/exception.hpp>

using namespace Amber;
using namespace Common;

std::string_view RecorderChannelDescription::GetName() const noexcept
{
	return m_Name;
}

size_t RecorderChannelDescription::GetSize() const noexcept
{
	size_t size = 0;
	for (size_t member_index = 0; member_index < GetMemberCount(); ++member_index)
	{
		size += GetMember(member_index).GetSize();
	}
	return size;
}

size_t RecorderChannelDescription::GetMemberCount() const noexcept
{
	return m_Members.size();
}

std::optional<size_t> RecorderChannelDescription::FindMember(std::string_view a_Name) const noexcept
{
	const auto it = m_MemberNameMap.find(a_Name);
	if (it != m_MemberNameMap.end())
	{
		return it->second;
	}
	else
	{
		return {};
	}
}

const RecorderMemberDescription& RecorderChannelDescription::GetMember(size_t a_Index) const noexcept
{
	return m_Members[a_Index];
}

void RecorderChannelDescription::SetName(std::string_view a_Name)
{
	m_Name = a_Name;
}

size_t RecorderChannelDescription::AddMember(const RecorderMemberDescription& a_Description)
{
	// Check if the name is not of zero length
	if (a_Description.GetName().size() == 0)
	{
		throw Exception("Invalid member name");
	}

	// Check if the channel already exists
	if (FindMember(a_Description.GetName()).has_value())
	{
		throw Exception("Member with this name already exists");
	}

	// Reserve all memory before changing containers for strong exception guarantee
	auto description = a_Description; // Copy description
	m_Members.reserve(m_Members.size() + 1); // Allocate channel
	const size_t channel_index = m_Members.size();

	// Add to name map
	m_MemberNameMap.emplace(description.GetName(), channel_index);

	// Add to channel list
	m_Members.emplace_back(std::move(description));

	// Return the index of the new channel
	return channel_index;
}