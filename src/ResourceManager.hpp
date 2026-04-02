#ifndef __RPS_RESOURCE_MANAGER_HPP
#define __RPS_RESOURCE_MANAGER_HPP


#include <memory>
#include <string>
#include <unordered_map>

#include <SFML/Graphics/Texture.hpp>

template<typename T>
class ResourceManager
{
protected:
	std::unordered_map<std::string, std::shared_ptr<T>> resources;
	std::string resource_path = "assets";

public:
	virtual ~ResourceManager() = default;

	std::shared_ptr<T> load(const std::string& p_id, const std::string& p_filename)
	{
		auto it = resources.find(p_id);

		if(it != resources.end())
		{
			return it->second;
		}

		auto resource = std::make_shared<T>();
		if(!load_from_file(resource.get(), resource_path + p_filename))
		{
			throw std::runtime_error("Failed to load: " + filename)
		}

		resource[id] = resource;
		return resource;
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

	virtual bool load_from_file(T* p_resource, const std::string& p_path) = 0;
};

class TextureManager: public ResourceManager<sf::Texture>
{
public:
	bool load_from_file(sf::Texture* p_texture, const std::string& p_path) override;
};

#endif
