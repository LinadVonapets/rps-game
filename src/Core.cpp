#include "Core.hpp"

#include <imgui.h>
#include <imgui-SFML.h>

#include <toml++/toml.hpp>

Core* Core::get()
{
	if(instance == nullptr)
	{
		instance = std::make_unique<Core>();
	}
	return instance.get();
}

Core::Core()
	:
	window_title{"rps_life"},
	window_rect({0, 0}, {1280, 720}),
	window(sf::VideoMode(window_rect.size), window_title)
{
	this->load_media();
	// this->window.setVerticalSyncEnabled(true);
	this->spawn_type = Entity::ROCK;
	this->delta_time = sf::Time::Zero;
}

void Core::run()
{
	this->setup();
	while(this->window.isOpen())
	{
		this->count_dt();
		while(const std::optional event = window.pollEvent())
			this->process_events(event);

		user_interface->update(this->delta_time);
		entity_group_system.update(this->delta_time);

		window.clear(sf::Color::White);

		window.draw(entity_group_system);

		user_interface->display();
		window.display();
		this->count_fps();
	}
}

const sf::Rect<std::uint32_t>& Core::get_window_rect()
{
	return this->window_rect;
}

GUI& Core::get_user_interface()
{
	return *this->user_interface;
}

EntityGroupSystem& Core::get_entity_group_system()
{
	return this->entity_group_system;
}

void Core::message_to_title(sf::String p_message)
{
	this->window.setTitle(this->window_title + ": [" + p_message + "]");
}

void Core::message_to_stdout(sf::String p_message)
{
	std::cout << p_message.toAnsiString() << std::endl;
}

void Core::message_to_all_output(sf::String p_message)
{
	this->message_to_title(p_message);
	this->message_to_stdout(p_message);
};

double Core::get_fps(sf::Time refresh_rate)
{
	if (this->fps_refresh_timer.getElapsedTime() >= refresh_rate)
	{
		this->fps = this->real_fps;
		this->fps_refresh_timer.restart();
	}
	return this->fps;
}

std::shared_ptr<sf::Texture> Core::get_texture(Entity::Type p_type)
{
	std::string id = this->entity_enum_to_str(p_type);
	return this->texture_manager.get(id);
}

std::shared_ptr<sf::SoundBuffer> Core::get_sound(Entity::Type p_type)
{
	std::string id = this->entity_enum_to_str(p_type);
	return this->sound_manager.get(id);
}

std::shared_ptr<sf::Font> Core::get_font(const std::string& p_id)
{
	return this->font_manager.get(p_id);
}

std::string Core::entity_enum_to_str(Entity::Type p_type)
{
	std::string type = "lost";
	switch(p_type)
	{
	case Entity::Type::PAPER:
		type = "paper";
		break;
	case Entity::Type::ROCK:
		type = "rock";
		break;
	case Entity::Type::SCISSORS:
		type = "scissors";
		break;
	default:
		type = "lost";
	}
	return type;
}

void Core::count_dt()
{
	this->delta_time = this->delta_clock.restart();
}

void Core::count_fps()
{
	float dt = this->delta_time.asSeconds();
	if (dt > 0)
		this->real_fps = 1 / dt;
}

void Core::load_media()
{
	std::filesystem::path assets_dir = "assets";
	std::fstream file(assets_dir / "assets.toml", std::ios::in);

	toml::table tbl = toml::parse(file);

	std::optional<std::string> sounds_dir = tbl["sounds"]["dir"].value<std::string>();
	std::optional<std::string> images_dir = tbl["images"]["dir"].value<std::string>();
	std::optional<std::string> fonts_dir = tbl["fonts"]["dir"].value<std::string>();


	std::filesystem::path image_path[Entity::Type::ALL] =
	{
		"assets/images/rock.png",
		"assets/images/paper.png",
		"assets/images/scissors.png",
		"assets/images/lost.png",
	};

	std::filesystem::path sound_path[Entity::Type::ALL] =
	{
		"assets/sounds/rock.wav",
		"assets/sounds/paper.wav",
		"assets/sounds/scissors.wav",
		"assets/sounds/lost.wav"
	};

	for(size_t i = 0; i < Entity::Type::ALL; i++)
	{
		texture_manager.load(image_path[i].stem(), image_path[i]);
		sound_manager.load(sound_path[i].stem(), sound_path[i]);
	}

	font_manager.load("fixedsys", "assets/fonts/fixedsys.ttf");
}

void Core::process_events(const std::optional<sf::Event> p_event)
{
	user_interface->process_events(p_event);
	if (p_event->is<sf::Event::Closed>())
	{
		this->window.close();
	}
	else if (const auto* MouseKey = p_event->getIf<sf::Event::MouseButtonPressed>())
	{
		if ((MouseKey->button == sf::Mouse::Button::Left) && !user_interface->is_want_capture_mouse())
		{
			entity_group_system.spawn_group(
				user_interface->get_entity_type(),
				user_interface->quantity,
				sf::Vector2f(MouseKey->position),
				user_interface->spawn_area_radius
			);
		}
	}
}

void Core::setup()
{
	this->user_interface = std::make_unique<GUI>(window);

	int group_radius = 125;

	entity_group_system.spawn_group(Entity::SCISSORS, 20, {100, 300}, group_radius);
	entity_group_system.spawn_group(Entity::PAPER, 20, {350, 100}, group_radius);
	entity_group_system.spawn_group(Entity::ROCK, 20, {700, 400}, group_radius);
}
