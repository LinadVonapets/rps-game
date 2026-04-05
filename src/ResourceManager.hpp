#ifndef __RPS_RESOURCE_MANAGER_HPP
#define __RPS_RESOURCE_MANAGER_HPP


#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

template<typename T>
class ResourceManager
{
protected:
	std::unordered_map<std::string, std::shared_ptr<T>> resources;

public:
	virtual ~ResourceManager() = default;

	std::shared_ptr<T> load(const std::string& p_id, const std::string& p_filename)
	{
		auto it = resources.find(p_id);

		if(it != resources.end())
		{
			return it->second;
		}

		auto res = std::make_shared<T>();
		if(!load_from_file(res.get(), p_filename))
		{
			throw std::runtime_error("Failed to load: " + p_filename);
		}

		resources[p_id] = res;
		return res;
	}

	std::shared_ptr<T> get(const std::string& p_id)
	{
		auto it = resources.find(p_id);
		if(it != resources.end())
		{
			return it->second;
		}
		return nullptr;
	}

	void unload(const std::string& p_id, bool force=false)
	{
		auto it = resources.find(p_id);
		long use_count = it->second.use_count();
		if (use_count > 1 && !force)
		{
			std::cerr << "Failed: Cannot delete: ";
			std::cerr << "The resource " << p_id << " is used by " << use_count << " objects" << std::endl;
		}
		resources.erase(it);
	}

	virtual bool load_from_file(T* p_resource, const std::string& p_path) = 0;
};

class TextureManager: public ResourceManager<sf::Texture>
{
public:
	bool load_from_file(sf::Texture* p_texture, const std::string& p_path) override;
};

class SoundManager: public ResourceManager<sf::SoundBuffer>
{
public:
	bool load_from_file(sf::SoundBuffer* p_sound_buffer, const std::string& p_path) override;
};

class FontManager: public ResourceManager<sf::Font>
{
	bool load_from_file(sf::Font* p_font, const std::string& p_path) override;
};

#endif
