#include "Core.hpp"

int main()
{
	Core& core = Core::get_instance();
	core.run();
}