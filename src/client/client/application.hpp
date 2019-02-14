#ifndef H_DEMU_CLIENT_APPLICATION
#define H_DEMU_CLIENT_APPLICATION

#include <client/api.hpp>

namespace Demu::Client
{
	class CLIENT_API Application
	{
		public:
		void Tick();
	};
}

#endif