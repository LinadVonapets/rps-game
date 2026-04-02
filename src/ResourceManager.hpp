#ifndef __RPS_RESOURCE_MANAGER_HPP
#define __RPS_RESOURCE_MANAGER_HPP


#include <memory>
#include <string>
#include <unordered_map>

template<typename T>
class ResourceManager
{
protected:
	std::unordered_map<std::string, std::shared_ptr<T>> resources;
	std::string resource_path;

public:
	virtual ~ResourceManager() = default;

	std::shared_ptr<T> load(const std::string& p_id, const std::string& p_path);

	std::shared_ptr<T> get(const std::string& p_id);

	virtual bool load_from_file(T* p_resource, const std::string& p_path)
};

class TextureManager : public ResourceManager<sf::Texture>
{
public:
	bool load_from_file(sf::Texture* texture, const std::string& path) override
	{
		return texture
	}
}

#include "ResourceManager.inl"

#endif
