#ifndef H_AMBER_GAMEBOY_CPU
#define H_AMBER_GAMEBOY_CPU

#include <gameboy/api.hpp>
#include <gameboy/extendedinstruction.hpp>
#include <gameboy/instruction.hpp>

#include <common/memory.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API CPU
	{
		public:
		CPU(Common::Memory16& a_Memory);

		Common::Memory16& GetMemory() const noexcept;

		void Tick();

		private:
		Common::Memory16& m_Memory;
	};
}

#endif