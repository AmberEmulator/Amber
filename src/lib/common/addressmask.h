#ifndef H_DEMU_COMMON_ADDRESSMASK
#define H_DEMU_COMMON_ADDRESSMASK

#include <cassert>
#include <cstdint>
#include <memory>
#include <type_traits>

namespace Demu::Common
{
	template <typename T>
	class AddressMask
	{
		public:
		using Address = T;
		static_assert(std::is_unsigned_v<Address> && !std::is_same_v<Address, bool>, "Address must be an unsigned integer");

		AddressMask(size_t a_BitCount):
			m_BitCount(a_BitCount),
			m_ByteCount((m_BitCount / 8) + (m_BitCount % 8 > 0 ? 1 : 0)),
			m_MaskBuffer(std::make_unique<uint8_t[]>(m_ByteCount * 2))
		{
			std::memset(m_MaskBuffer.get(), 0, m_ByteCount);
		}

		size_t GetBitCount() const noexcept
		{
			return m_BitCount;
		}

		bool GetReadBit(size_t a_Bit) const noexcept
		{
			return GetBit(a_Bit, GetReadBuffer());
		}

		bool GetWriteBit(size_t a_Bit) const noexcept
		{
			return GetBit(a_Bit, GetWriteBuffer());
		}

		void SetReadBit(size_t a_Bit, bool a_Value)
		{
			SetBit(a_Bit, GetReadBuffer(), a_Value);
		}

		void SetWriteBit(size_t a_Bit, bool a_Value)
		{
			SetBit(a_Bit, GetWriteBuffer(), a_Value);
		}

		void SetReadBits(size_t a_StartBit, size_t a_Count, bool a_Value)
		{
			for (size_t i = a_StartBit; i < a_Count; ++i)
			{
				SetReadBit(i, a_Value);
			}
		}

		void SetWriteBits(size_t a_StartBit, size_t a_Count, bool a_Value)
		{
			for (size_t i = a_StartBit; i < a_Count; ++i)
			{
				SetWriteBit(i, a_Value);
			}
		}

		private:
		const uint8_t* GetReadBuffer() const noexcept
		{
			return m_MaskBuffer.get();
		}

		const uint8_t* GetWriteBuffer() const noexcept
		{
			return m_MaskBuffer.get() + m_ByteCount;
		}

		uint8_t* GetReadBuffer() noexcept
		{
			return const_cast<uint8_t*>(static_cast<const AddressMask*>(this)->GetReadBuffer());
		}

		uint8_t* GetWriteBuffer() noexcept
		{
			return const_cast<uint8_t*>(static_cast<const AddressMask*>(this)->GetWriteBuffer());
		}

		bool GetBit(size_t a_Bit, const uint8_t* a_Buffer) const noexcept
		{
			assert(a_Bit < m_BitCount && "Invalid bit index");

			const size_t byte = a_Bit / 8;
			const size_t bit = a_Bit % 8;
			const uint8_t mask = static_cast<uint8_t>(1 << bit);

			return (a_Buffer[byte] & mask) != 0;
		}

		void SetBit(size_t a_Bit, uint8_t* a_Buffer, bool a_Value) noexcept
		{
			assert(a_Bit < m_BitCount && "Invalid bit index");

			const size_t byte = a_Bit / 8;
			const size_t bit = a_Bit % 8;
			const uint8_t mask = static_cast<uint8_t>(1 << bit);

			if (a_Value)
			{
				a_Buffer[byte] |= mask;
			}
			else
			{
				a_Buffer[byte] &= ~mask;
			}
		}

		size_t m_BitCount;
		size_t m_ByteCount;
		std::unique_ptr<uint8_t[]> m_MaskBuffer;
	};
}

#endif