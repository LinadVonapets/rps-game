#ifndef __RPS_CORE_HPP
#define __RPS_CORE_HPP

#include <memory>
#include <SFML/Graphics.hpp>

class Core {
private:
	static inline Core* instance = nullptr;
	
// Do not change order of these fields, because we initialize them 
// in member initializer list exactly in the order they are declared here
	sf::Rect<std::uint32_t> window_rect;
	sf::RenderWindow window;

public:
	static Core& get_instance();

	Core();
	virtual ~Core() = default;

	void run();
	sf::Rect<std::uint32_t> get_window_rect() {
		return window_rect;
	}

private:
	Core(const Core&) = delete;
	Core& operator=(const Core&) = delete;
};

#endif
