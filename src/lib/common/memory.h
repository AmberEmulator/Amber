#ifndef H_DEMU_COMMON_MEMORY
#define H_DEMU_COMMON_MEMORY

#include <common/addressmask.h>

#include <cstdint>
#include <type_traits>

namespace Demu::Common
{
	template <typename T>
	class Memory
	{
		using Address = T;
		static_assert(std::is_unsigned_v<Address> && !std::is_same_v<Address, bool>, "Address must be an unsigned integer");

		virtual ~Memory() noexcept = 0;

		virtual AddressMask<Address> GetAddressMask() const = 0;

		virtual uint8_t  Load8 (Address a_Address) const = 0;
		virtual uint16_t Load16(Address a_Address) const = 0;
		virtual uint32_t Load32(Address a_Address) const = 0;
		virtual uint64_t Load64(Address a_Address) const = 0;

		virtual void Store8 (Address a_Address, uint8_t  a_Value) = 0;
		virtual void Store16(Address a_Address, uint16_t a_Value) = 0;
		virtual void Store32(Address a_Address, uint32_t a_Value) = 0;
		virtual void Store64(Address a_Address, uint64_t a_Value) = 0;
	};

	template <typename T>
	Memory<T>::~Memory() noexcept = default;

	template <typename T, bool BE>
	class MemoryHelper : public Memory
	{
		constexpr bool IsBigEndian = BE;

		virtual uint16_t Load16(Address a_Address) const override
		{
			if constexpr (IsBigEndian)
			{
				return (Load8(a_Address) << 8) | Load8(a_Address + 1);
			}
			else
			{
				return (Load8(a_Address + 1) << 8) | Load8(a_Address);
			}
		}

		virtual uint32_t Load32(Address a_Address) const override
		{
			if constexpr (IsBigEndian)
			{
				return (Load16(a_Address) << 16) | Load16(a_Address + 2);
			}
			else
			{
				return (Load16(a_Address + 2) << 16) | Load16(a_Address);
			}
		}

		virtual uint64_t Load64(Address a_Address) const override
		{
			if constexpr (IsBigEndian)
			{
				return (Load32(a_Address) << 32) | Load32(a_Address + 4);
			}
			else
			{
				return (Load32(a_Address + 4) << 32) | Load32(a_Address);
			}
		}

		virtual void Store16(Address a_Address, uint16_t a_Value) override
		{
			if constexpr (IsBigEndian)
			{
				Store8(a_Address + 1, (a_Value >> 8) & 0xFF);
				Store8(a_Address, a_Value & 0xFF);
			}
			else
			{
				Store8(a_Address, (a_Value >> 8) & 0xFF);
				Store8(a_Address + 1, a_Value & 0xFF);
			}
		}

		virtual void Store32(Address a_Address, uint32_t a_Value) override
		{
			if constexpr (IsBigEndian)
			{
				Store16(a_Address + 2, (a_Value >> 16) & 0xFFFF);
				Store16(a_Address, a_Value & 0xFFFF);
			}
			else
			{
				Store16(a_Address, (a_Value >> 16) & 0xFFFF);
				Store16(a_Address + 2, a_Value & 0xFFFF);
			}
		}

		virtual void Store64(Address a_Address, uint64_t a_Value) override
		{
			if constexpr (IsBigEndian)
			{
				Store32(a_Address + 4, (a_Value >> 32) & 0xFFFFFFFF);
				Store32(a_Address, a_Value & 0xFFFFFFFF);
			}
			else
			{
				Store32(a_Address, (a_Value >> 32) & 0xFFFFFFFF);
				Store32(a_Address + 4, a_Value & 0xFFFFFFFF);
			}
		}
	};
}

#endif