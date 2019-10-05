#ifndef H_AMBER_COMMON_RECORDERDESCRIPTION
#define H_AMBER_COMMON_RECORDERDESCRIPTION

#include <common/api.hpp>
#include <common/recorderchanneldescription.hpp>

#include <map>
#include <optional>
#include <vector>

namespace Amber::Common
{
	class COMMON_API RecorderDescription
	{
		public:
		size_t GetChannelCount() const noexcept;
		std::optional<size_t> FindChannel(std::string_view a_Name) const noexcept;
		const RecorderChannelDescription& GetChannel(size_t a_Index) const noexcept;
		size_t AddChannel(const RecorderChannelDescription& a_Description);

		size_t GetBlockSize() const noexcept;

		void SetBlockSize(size_t a_Size) noexcept;

		private:
		std::vector<RecorderChannelDescription> m_Channels;
		std::map<std::string, size_t, std::less<>> m_ChannelNameMap;
		size_t m_BlockSize = 1024 * 1024 * 1024; // 1mb
	};
}

#endif