#ifndef H_AMBER_CLIENT_TEXTURE
#define H_AMBER_CLIENT_TEXTURE

#include <client/api.hpp>
namespace Amber::Client
{
	class CLIENT_API Texture
	{
		public:
		using NativeHandle = uint32_t;

		Texture(size_t a_Width, size_t a_Height);
		Texture(const Texture& a_Texture) = delete;
		~Texture() noexcept;

		NativeHandle GetNativeHandle() const noexcept;

		void Blit(size_t a_X, size_t a_Y, size_t a_Width, size_t a_Height, const void* a_Data);

		Texture& operator = (const Texture& a_Texture) = delete;

		private:
		NativeHandle m_NativeHandle;
	};
}

#endif