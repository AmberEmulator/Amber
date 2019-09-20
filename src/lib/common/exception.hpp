#ifndef H_AMBER_EXCEPTION
#define H_AMBER_EXCEPTION

#include <common/api.hpp>

#include <stdexcept>

namespace Amber
{
	class COMMON_API Exception : public std::runtime_error
	{
		public:
		Exception(std::string_view a_Message);
	};
}

#endif