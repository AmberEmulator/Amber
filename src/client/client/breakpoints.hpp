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
		Common::Breakpoint m_Selected;

		Common::BreakpointDescription m_NewBreakpointDescription;
	};

	CLIENT_API void ShowBreakpoints(const char* a_Name, BreakpointsState& a_State);
}

#endif