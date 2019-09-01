#ifndef H_AMBER_CLIENT_BREAKPOINTS
#define H_AMBER_CLIENT_BREAKPOINTS

#include <client/api.hpp>

#include <common/debugger.hpp>

#include <optional>

namespace Amber::Client
{
	struct BreakpointsState
	{
		Common::Debugger* m_Debugger = nullptr;
		std::optional<uint64_t> m_Selected;
		uint64_t m_NewAddress = 0;
	};

	CLIENT_API void ShowBreakpoints(const char* a_Name, BreakpointsState& a_State);
}

#endif