#ifndef H_DEMU_COMMON_DISASSEMBLY
#define H_DEMU_COMMON_DISASSEMBLY

#include <common/api.hpp>

namespace Demu::Common
{
	class COMMON_API Disassembly
	{
		public:
		virtual ~Disassembly() noexcept = 0;

		virtual uint64_t GetMaximumAddress() const noexcept;
		virtual size_t GetMaximumAddressWidth() const noexcept;

		virtual bool IsValidAddress(uint64_t a_Address) const noexcept = 0;
		virtual uint64_t GetInstructionSize(uint64_t a_Address) const = 0;
		virtual std::string GetAddressName(uint64_t a_Address) const;
		virtual std::string GetInstructionName(uint64_t a_Address) const = 0;

		virtual bool HasBreakpoint(uint64_t a_Address) const noexcept;
		virtual void SetBreakpoint(uint64_t a_Address, bool a_Enabled);
	};
}

#endif