#ifndef H_DEMU_CLIENT_DISASSEMBLY
#define H_DEMU_CLIENT_DISASSEMBLY

#include <client/api.hpp>

#include <common/disassembly.hpp>

namespace Demu::Client
{
	struct DisassemblyState
	{
		Common::Disassembly* m_Disassembly = nullptr;
		uint64_t m_ViewAddress = 0;
		uint64_t m_SelectedAddress = 0;
	};

	CLIENT_API void ShowDisassembly(const char* a_Name, DisassemblyState& a_State);
}

#endif