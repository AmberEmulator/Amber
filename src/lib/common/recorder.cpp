#include <common/recorder.hpp>

#include <set>

using namespace Amber;
using namespace Common;

Recorder::Recorder(const RecorderDescription& a_Description):
	m_Description(a_Description)
{
	for (size_t i = 0; i < m_Description.GetChannelCount(); ++i)
	{
		const size_t channel_size = m_Description.GetChannel(i).GetSize();

		ChannelInfo channel_info{};
		channel_info.m_Offset = m_FrameSize;
		channel_info.m_Size = channel_size;
		m_Channels.push_back(channel_info);

		m_FrameSize += channel_size;
	}

	m_CurrentFrameState = std::make_unique<uint8_t[]>(m_FrameSize);
	m_LastFrameState = std::make_unique<uint8_t[]>(m_FrameSize);
	std::memset(m_CurrentFrameState.get(), 0, m_FrameSize);
	std::memset(m_LastFrameState.get(), 0, m_FrameSize);

	AllocateNewBuffer();
}

const RecorderDescription& Recorder::GetDescription() const noexcept
{
	return m_Description;
}

size_t Recorder::GetFrameCount() const noexcept
{
	return m_Frames.size();
}

size_t Recorder::NewFrame()
{
	Frame frame = {};
	if (m_Buffers.back().m_Size - m_Buffers.back().m_Used < m_FrameSize)
	{
		AllocateNewBuffer();
	}

	auto& buffer = m_Buffers.back();
	frame.m_Data = buffer.m_Data.get() + buffer.m_Used;
	std::memcpy(frame.m_Data, m_CurrentFrameState.get(), m_FrameSize);
	buffer.m_Used += m_FrameSize;

	const size_t frame_index = m_Frames.size();
	m_Frames.push_back(frame);

	m_CurrentFrame = frame_index;
	std::memcpy(m_LastFrameState.get(), m_CurrentFrameState.get(), m_FrameSize);

	return frame_index;
}

size_t Recorder::GetCurrentFrame() const noexcept
{
	return m_CurrentFrame;
}

void Recorder::SetCurrentFrame(size_t a_Frame) noexcept
{
	if (m_CurrentFrame == a_Frame)
	{
		return;
	}

	m_CurrentFrame = a_Frame;
	auto& current_frame = m_Frames[m_CurrentFrame];
	std::memcpy(m_CurrentFrameState.get(), current_frame.m_Data, m_FrameSize);
}

void Recorder::WriteChannelData(size_t a_Channel, const void* a_Data)
{
	auto& channel_info = m_Channels[a_Channel];
	std::memcpy(m_CurrentFrameState.get() + channel_info.m_Offset, a_Data, channel_info.m_Size);
	channel_info.m_Dirty = true;
}

const void* Recorder::ReadChannelData(size_t a_Channel) const noexcept
{
	const auto& channel_info = m_Channels[a_Channel];
	return m_CurrentFrameState.get() + channel_info.m_Offset;
}

void Recorder::AllocateNewBuffer()
{
	Buffer buffer = {};
	buffer.m_Size = m_Description.GetBlockSize();
	buffer.m_Data = std::make_unique<uint8_t[]>(buffer.m_Size);

	m_Buffers.emplace_back(std::move(buffer));
}