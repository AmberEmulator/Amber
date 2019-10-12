#ifndef H_AMBER_CLIENT_APPLICATION
#define H_AMBER_CLIENT_APPLICATION

#include <client/api.hpp>
#include <client/configuration.hpp>

#include <gameboy/cartridge.hpp>
#include <gameboy/device.hpp>

namespace Amber::Client
{
	class Texture;

	class CLIENT_API Application
	{
		public:
		Application();
		~Application();

		void Tick();

		private:
		void CreateDevice();
		void LoadCartridge(const std::filesystem::path& a_Cartridge);

		void OpenError(const char* a_Message);
		void ShowError();
		void ShowDockspace();
		void ShowMenu();
		void ShowLCD();

		// Device
		std::unique_ptr<Gameboy::Device> m_Device;
		std::unique_ptr<Gameboy::Cartridge> m_Cartridge;
		std::unique_ptr<Texture> m_LCDTexture;
		std::unique_ptr<uint8_t[]> m_LCDBuffer;

		// Config
		std::filesystem::path m_ApplicationDirectory;
		Configuration m_Configuration;
		std::string m_INIPath;

		// UI
		std::string m_ErrorMessage;

	};
}

#endif