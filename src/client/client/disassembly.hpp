#ifndef H_AMBER_CLIENT_DISASSEMBLY
#define H_AMBER_CLIENT_DISASSEMBLY

#include <client/api.hpp>

#include <common/debugger.hpp>

namespace Amber::Client
{
	struct DisassemblyState
	{
		Common::Debugger* m_Debugger = nullptr;
		uint64_t m_ViewAddress = 0;
		uint64_t m_SelectedAddress = 0;
		bool m_Fixed = false;
		uint16_t m_FixedAddress = 0;
	};

	CLIENT_API void ShowDisassembly(const char* a_Name, DisassemblyState& a_State);
}

#endif