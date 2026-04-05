#include "ResourceManager.hpp"

bool TextureManager::load_from_file(sf::Texture* p_texture, const std::string& p_path)
{
	return p_texture->loadFromFile(p_path);
}

bool SoundManager::load_from_file(sf::SoundBuffer* p_sound_buffer, const std::string& p_path)
{
	return p_sound_buffer->loadFromFile(p_path);
}

bool FontManager::load_from_file(sf::Font* p_font, const std::string& p_path)
{
	return p_font->openFromFile(p_path);
}
