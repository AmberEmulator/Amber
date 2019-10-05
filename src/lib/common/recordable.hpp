#ifndef H_AMBER_COMMON_RECORDABLE
#define H_AMBER_COMMON_RECORDABLE

#include <common/api.hpp>
#include <common/recordermemberdescription.hpp>

namespace Amber::Common
{
	class COMMON_API Recordable
	{
		public:
		virtual ~Recordable() noexcept = 0;

		virtual size_t GetRecorderMemberCount() const noexcept = 0;
		virtual const RecorderMemberDescription& GetRecorderMember(size_t a_Index) const noexcept = 0;
	};
}

#endif