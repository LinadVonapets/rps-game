#include "ResourceManager.hpp"

bool TextureManager::load_from_file(sf::Texture* p_texture, const std::string& p_path)
{
	return p_texture->loadFromFile(p_path);
}
