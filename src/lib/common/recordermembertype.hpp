#ifndef H_AMBER_COMMON_RECORDERMEMBERTYPE
#define H_AMBER_COMMON_RECORDERMEMBERTYPE

#include <common/api.hpp>

namespace Amber::Common
{
	namespace RecorderMemberType
	{
		enum Enum
		{
			uint8,
			uint16,
			uint32,
			uint64,

			Raw
		};
	}
}

#endif