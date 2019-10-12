#ifdef _WIN32

#include <client/path.hpp>

#include <Shlobj.h>
#include <Windows.h>

using namespace Amber;
using namespace Client;

std::filesystem::path Amber::Client::Path::GetUserApplicationDirectory()
{
	PWSTR buffer;

	const HRESULT result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, NULL, &buffer);
	if (FAILED(result))
	{
		throw std::runtime_error("Failed to get known user application directory");
	}

	auto directory = std::filesystem::path(buffer);

	// This can leak on exception, WTB scopeguard
	CoTaskMemFree(buffer);

	return directory;
}

std::optional<std::filesystem::path> Amber::Client::Path::ShowOpenFileDialog()
{
	OPENFILENAMEW open_filename{};
	wchar_t filename_buffer[4096] = {};
	open_filename.lStructSize = sizeof(open_filename);
	open_filename.lpstrFile = filename_buffer;
	open_filename.nMaxFile = sizeof(filename_buffer);
	open_filename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileNameW(&open_filename))
	{
		return {};
	}

	return std::filesystem::path(filename_buffer);
}

#endif