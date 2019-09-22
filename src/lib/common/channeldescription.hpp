#ifndef H_AMBER_COMMON_CHANNELDESCRIPTION
#define H_AMBER_COMMON_CHANNELDESCRIPTION

#include <common/api.hpp>

namespace Amber::Common
{
	class COMMON_API ChannelDescription
	{
		public:
		std::string_view GetName() const noexcept;
		size_t GetSize() const noexcept;

		void SetName(std::string_view a_Name);
		void SetSize(size_t a_Size) noexcept;

		private:
		std::string m_Name;
		size_t m_Size = 0;
	};
}

#endif