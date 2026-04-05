#ifndef __RPS_CORE_HPP
#define __RPS_CORE_HPP

#include <memory>

#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "EntityGroupSystem.hpp"
#include "GUI.hpp"
#include "ResourceManager.hpp"

class Core
{
private:
	static inline Core* instance = nullptr;

	//----------------------------------------------------------------//
	// Do not change order of these fields, because we initialize them
	// in member initializer list exactly in the order they are declared here

	sf::String window_title;
	sf::Rect<std::uint32_t> window_rect;
	sf::RenderWindow window;
	//----------------------------------------------------------------//
	std::unique_ptr<GUI> user_interface;

	TextureManager texture_manager;
	FontManager font_manager;
	SoundManager sound_manager;

	EntityGroupSystem entity_group_system;
	Entity::Type spawn_type;


	sf::Clock delta_clock;
	sf::Time delta_time;

	sf::Clock fps_refresh_timer;
	double real_fps;
	double fps;

public:
	static Core& get_instance();

	Core();
	virtual ~Core() = default;

	void run();
	const sf::Rect<std::uint32_t>& get_window_rect();
	GUI& get_user_interface();
	EntityGroupSystem& get_entity_group_system();

	void message_to_title(sf::String p_message);
	void message_to_stdout(sf::String p_message);
	void message_to_all_output(sf::String p_message);

	double get_fps(sf::Time refresh_rate);

	std::shared_ptr<sf::Texture> get_texture(Entity::Type p_type);
	std::shared_ptr<sf::SoundBuffer> get_sound(Entity::Type p_type);
	std::shared_ptr<sf::Font> get_font(const std::string& p_id);

	std::string entity_enum_to_str(Entity::Type p_type);

private:
	void count_dt();
	void count_fps();
	void load_media();

	void process_events(const std::optional<sf::Event> p_event);

	Core(const Core&) = delete;
	Core& operator=(const Core&) = delete;
};

#endif
