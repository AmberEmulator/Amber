#ifndef H_AMBER_COMMON_REGISTER
#define H_AMBER_COMMON_REGISTER

#include <gameboy/api.hpp>

#include <memory>
#include <type_traits>

namespace Amber::Common
{
	namespace Internal
	{
		template <typename T>
		class RegisterStorage
		{
			protected:
			template <typename U>
			void LoadBlock(U* a_Destination, uint8_t a_Index) const noexcept
			{
				// TODO: big endian support
				constexpr size_t start = sizeof(T) - sizeof(U);
				std::memcpy(a_Destination, (m_Data + start) - sizeof(U) * a_Index, sizeof(U));
			}

			template <typename U>
			void StoreBlock(const U* a_Source, uint8_t a_Index) noexcept
			{
				// TODO: big endian support
				constexpr size_t start = sizeof(T) - sizeof(U);
				std::memcpy((m_Data + start) - sizeof(U) * a_Index, a_Source, sizeof(U));
			}

			uint8_t m_Data[sizeof(T)] = {};
		};


		template <typename T>
		class RegisterIndexed16 : public RegisterStorage<T>
		{
			public:
			uint16_t Load16(uint8_t a_Index) const noexcept
			{
				uint16_t value;
				LoadBlock(&value, a_Index);
				return value;
			}

			void Store16(uint8_t a_Index, uint16_t a_Value) noexcept
			{
				StoreBlock(&a_Value, a_Index);
			}
		};

		template <typename T>
		class RegisterNotIndexed16 : public RegisterIndexed16<T>
		{
			public:
			using RegisterIndexed16::Load16;
			uint16_t Load16() const noexcept
			{
				return RegisterIndexed16::Load16(0);
			}

			using RegisterIndexed16::Store16;
			void Store16(uint16_t a_Value) noexcept
			{
				return RegisterIndexed16::Store16(0, a_Value);
			}
		};

		template <typename T>
		class RegisterIndexed8 : public std::conditional_t<sizeof(T) == 1, RegisterStorage<T>, std::conditional_t<sizeof(T) == 2, RegisterNotIndexed16<T>, RegisterIndexed16<T>>>
		{
			public:
			uint8_t Load8(uint8_t a_Index) const noexcept
			{
				return m_Data[(sizeof(T) - 1) - a_Index];
			}

			void Store8(uint8_t a_Index, uint8_t a_Value) noexcept
			{
				m_Data[(sizeof(T) - 1) - a_Index] = a_Value;
			}
		};

		template <typename T>
		class RegisterNotIndexed8 : public RegisterIndexed8<T>
		{
			public:
			using RegisterIndexed8::Load8;
			uint8_t Load8() const noexcept
			{
				return RegisterIndexed8::Load8(0);
			}

			using RegisterIndexed8::Store8;
			void Store8(uint8_t a_Value) noexcept
			{
				return RegisterIndexed8::Store8(0, a_Value);
			}
		};
	}

	template <typename T>
	class Register : public std::conditional_t<sizeof(T) == 1, Internal::RegisterNotIndexed8<T>, Internal::RegisterIndexed8<T>>
	{
	};

	using Register8  = Register<uint8_t>;
	using Register16 = Register<uint16_t>;
	using Register32 = Register<uint32_t>;
	using Register64 = Register<uint64_t>;
}

#endif