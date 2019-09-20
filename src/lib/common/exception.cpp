#include <common/exception.hpp>

using namespace Amber;

Exception::Exception(std::string_view a_Message):
	std::runtime_error(std::string(a_Message))
{
}