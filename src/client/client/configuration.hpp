#ifndef H_AMBER_CLIENT_CONFIGURATION
#define H_AMBER_CLIENT_CONFIGURATION

#include <client/api.hpp>
#include <client/path.hpp>

#include <deque>
#include <filesystem>

namespace Amber::Client
{
	class CLIENT_API Configuration
	{
		public:
		void Save(const std::filesystem::path& a_Path);
		void Load(const std::filesystem::path& a_Path);

		size_t GetRecentROMCount() const noexcept;
		const std::filesystem::path& GetRecentROM(size_t a_Index) const noexcept;

		void PushRecentROM(const std::filesystem::path& a_ROM);

		private:
		std::deque<std::filesystem::path> m_RecentROMs;
	};
}

#endif