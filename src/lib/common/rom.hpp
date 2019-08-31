#ifndef H_AMBER_COMMON_ROM
#define H_AMBER_COMMON_ROM

#include <common/memory.hpp>

#include <memory>

namespace Amber::Common
{
	template <typename T, bool BE>
	class ROM : public MemoryHelper<T, BE>
	{
		public:
		explicit ROM(size_t a_BankSize, size_t a_BankCount):
			m_BankSize(a_BankSize),
			m_BankCount(a_BankCount),
			m_Data(std::make_unique<uint8_t[]>(m_BankSize* m_BankCount))
		{
			std::memset(m_Data.get(), 0, m_BankSize * m_BankCount);
		}

		size_t GetBankSize() const noexcept
		{
			return m_BankSize;
		}

		size_t GetBankCount() const noexcept
		{
			return m_BankCount;
		}

		size_t GetSize() const noexcept
		{
			return GetBankSize() * GetBankCount();
		}

		size_t GetActiveBank() const noexcept
		{
			return m_ActiveBank;
		}

		uint8_t* GetData() noexcept
		{
			return const_cast<uint8_t*>(static_cast<const ROM*>(this)->GetData());
		}

		const uint8_t* GetData() const noexcept
		{
			return m_Data.get();
		}

		uint8_t* GetBankData(size_t a_Bank) noexcept
		{
			return const_cast<uint8_t*>(static_cast<const ROM*>(this)->GetBankData(a_Bank));
		}

		const uint8_t* GetBankData(size_t a_Bank) const noexcept
		{
			return GetData() + m_BankSize * a_Bank;
		}

		uint8_t* GetActiveBankData() noexcept
		{
			return const_cast<uint8_t*>(static_cast<const ROM*>(this)->GetActiveBankData());
		}

		const uint8_t* GetActiveBankData() const noexcept
		{
			return GetBankData(GetActiveBank());
		}

		void SetActiveBank(size_t a_Bank)
		{
			m_ActiveBank = a_Bank;
		}

		uint8_t Load8(Address a_Address) const override
		{
			return GetActiveBankData()[a_Address];
		}

		void Store8(Address a_Address, uint8_t a_Value) override
		{
		}

		private:
		const size_t m_BankSize;
		const size_t m_BankCount;
		size_t m_ActiveBank = 0;
		std::unique_ptr<uint8_t[]> m_Data;
	};

	template <bool BE> using ROM8  = ROM<uint8_t, BE>;
	template <bool BE> using ROM16 = ROM<uint16_t, BE>;
	template <bool BE> using ROM32 = ROM<uint32_t, BE>;
	template <bool BE> using ROM64 = ROM<uint64_t, BE>;
}

#endif