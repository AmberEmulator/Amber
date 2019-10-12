#ifndef H_AMBER_CLIENT_APPLICATION
#define H_AMBER_CLIENT_APPLICATION

#include <client/api.hpp>
#include <client/configuration.hpp>

namespace Amber::Client
{
	class CLIENT_API Application
	{
		public:
		Application();
		~Application();

		void Tick();

		private:
		void ShowMenu();

		std::filesystem::path m_ApplicationDirectory;
		Configuration m_Configuration;
		std::string m_INIPath;
	};
}

#endif