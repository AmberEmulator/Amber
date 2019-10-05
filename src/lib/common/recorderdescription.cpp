#include <common/recorderdescription.hpp>

#include <common/exception.hpp>

using namespace Amber;
using namespace Common;

size_t RecorderDescription::GetChannelCount() const noexcept
{
	return m_Channels.size();
}

std::optional<size_t> RecorderDescription::FindChannel(std::string_view a_Name) const noexcept
{
	const auto it = m_ChannelNameMap.find(a_Name);
	if (it != m_ChannelNameMap.end())
	{
		return it->second;
	}
	else
	{
		return {};
	}
}

const RecorderChannelDescription& RecorderDescription::GetChannel(size_t a_Index) const noexcept
{
	return m_Channels[a_Index];
}

size_t RecorderDescription::AddChannel(const RecorderChannelDescription& a_Description)
{
	// Check if the name is not of zero length
	if (a_Description.GetName().size() == 0)
	{
		throw Exception("Invalid channel name");
	}

	// Check if the channel already exists
	if (FindChannel(a_Description.GetName()).has_value())
	{
		throw Exception("Channel with this name already exists");
	}

	// Reserve all memory before changing containers for strong exception guarantee
	auto description = a_Description; // Copy description
	m_Channels.reserve(m_Channels.size() + 1); // Allocate channel
	const size_t channel_index = m_Channels.size();

	// Add to name map
	m_ChannelNameMap.emplace(description.GetName(), channel_index);

	// Add to channel list
	m_Channels.emplace_back(std::move(description));

	// Return the index of the new channel
	return channel_index;
}

size_t RecorderDescription::GetBlockSize() const noexcept
{
	return m_BlockSize;
}

void RecorderDescription::SetBlockSize(size_t a_Size) noexcept
{
	m_BlockSize = a_Size;
}