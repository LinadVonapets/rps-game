
#include "ResourceManager.hpp"

template<typename T>
std::shared_ptr<T> ResourceManager<T>::load(const std::string& p_id, const std::string& p_path)
{
	auto it = resources.find(id);
	if(it != resources.end())
	{
		return it->second;
	}
}

template<typename T>
std::shared_ptr<T> ResourceManager<T>::get(const std::string& p_id)
{

}
