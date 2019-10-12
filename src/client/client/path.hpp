#ifndef H_AMBER_CLIENT_PATH
#define H_AMBER_CLIENT_PATH

#include <client/api.hpp>

#include <filesystem>
#include <optional>

namespace Amber::Client
{
	namespace Path
	{
		std::filesystem::path GetUserApplicationDirectory();

		std::optional<std::filesystem::path> ShowOpenFileDialog();
	};
}

#endif