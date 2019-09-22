#ifndef H_AMBER_COMMON_RECORDER
#define H_AMBER_COMMON_RECORDER

#include <common/api.hpp>
#include <common/bytereader.hpp>
#include <common/bytewriter.hpp>
#include <common/recorderdescription.hpp>

#include <limits>
#include <memory>
#include <optional>
#include <vector>

namespace Amber::Common
{
	class COMMON_API Recorder
	{
		public:
		Recorder(const RecorderDescription& a_Description);

		const RecorderDescription& GetDescription() const noexcept;

		size_t GetFrameCount() const noexcept;
		size_t NewFrame();

		size_t GetCurrentFrame() const noexcept;
		void SetCurrentFrame(size_t a_Frame) noexcept;

		void WriteChannelData(size_t a_Channel, const void* a_Data);
		const void* ReadChannelData(size_t a_Channel) const noexcept;

		private:
		struct Frame
		{
			uint8_t* m_Data;
		};

		struct Buffer
		{
			std::unique_ptr<uint8_t[]> m_Data;
			size_t m_Size;
			size_t m_Used;
		};

		struct ChannelInfo
		{
			size_t m_Offset;
			size_t m_Size;
			bool m_Dirty;
		};

		static constexpr size_t InvalidFrame = std::numeric_limits<size_t>::max();

		//void PushData(size_t a_Channel, size_t a_Size);
		//void RestoreFrame(size_t a_Frame);

		void AllocateNewBuffer();

		const RecorderDescription m_Description;

		std::vector<Frame> m_Frames;
		std::vector<Buffer> m_Buffers;

		size_t m_FrameSize = 0;
		size_t m_CurrentFrame = 0;
		std::unique_ptr<uint8_t[]> m_CurrentFrameState;
		std::unique_ptr<uint8_t[]> m_LastFrameState;

		std::vector<ChannelInfo> m_Channels;
		//std::unique_ptr<uint8_t[]> m_WorkingBuffer;
	};
}

#endif