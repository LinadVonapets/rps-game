#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Entity.hpp"
#include "EntityGroupSystem.hpp"

#include "Core.hpp"

int main()
{
	Core& core = Core::get_instance();
	core.run();
}