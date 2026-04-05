#include "GUI.hpp"

#include <iostream>
#include <cmath>

#include <imgui.h>
#include <imgui-SFML.h>

#include "Core.hpp"

GUI::GUI(sf::RenderWindow& window)
	: window{window}
	, entity_count(*Core::get_instance().get_font("fixedsys"))
{
	if(!ImGui::SFML::Init(window))
	{
		std::string msg = "ImGui init failed! Exiting...";
		std::cerr << msg << std::endl;
		throw std::runtime_error(msg);
	}
	this->show = true;
	this->choice = Entity::Type::ROCK;

	this->spawn_area_radius = 0;
	this->spawn_area.setRadius(spawn_area_radius);
	this->spawn_area.setOrigin(this->spawn_area.getLocalBounds().getCenter());
	this->spawn_area.setFillColor(sf::Color::Transparent);
	this->spawn_area.setOutlineThickness(2.f);

	this->spawn_area.setOutlineColor(sf::Color(255, 0, 0, 128));

	this->clear_all_entity = false;

	this->entity_count.setFillColor(sf::Color::Black);
	this->entity_count.setCharacterSize(24);
}

void GUI::process_events(const std::optional<sf::Event> event)
{
	ImGui::SFML::ProcessEvent(this->window, *event);
	if (const auto* key = event->getIf<sf::Event::KeyPressed>())
	{
		if (key->scancode == sf::Keyboard::Scancode::M)
			this->show =! this->show;
	}
	else if (const auto mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
	{
		if (!this->is_want_capture_mouse())
		{
			if (mouseWheelScrolled->delta < 0)
				spawn_area_radius += 10;
			else
				spawn_area_radius -= 10;

			if (spawn_area_radius < 0)
				spawn_area_radius = 0;
			else if (spawn_area_radius > 300)
				spawn_area_radius = 300;

			this->spawn_area.setRadius(spawn_area_radius);
			this->spawn_area.setOrigin(this->spawn_area.getLocalBounds().getCenter());
		}
	}
}

void GUI::update(sf::Time dt)
{
	ImGui::SFML::Update(this->window, dt);
	this->spawn_area.setPosition(sf::Vector2f(sf::Mouse::getPosition(this->window)));

	if (this->quantity_mode == QuantityMode::PROPORTIONAL)
		this->quantity = spawn_area_radius * 1/8;

	if (this->quantity < 0)
		this->quantity = 0;
}

void GUI::display()
{
	int fps = static_cast<int>(Core::get()->get_fps(sf::seconds(2)));

	entity_count.setString(sf::String("FPS: ") + std::to_string(fps));
	entity_count.setPosition({0, 25});
	this->window.draw(entity_count);


	entity_count.setPosition({0, 0});
	entity_count.setString(sf::String("Entity count: ") + std::to_string(Core::get()->get_entity_group_system().get_next_id()));
	this->window.draw(entity_count);

	if(this->show)
		this->show_tools_menu();

	if (!this->is_want_capture_mouse())
		this->window.draw(spawn_area);

	ImGui::SFML::Render(this->window);
}

bool GUI::is_want_capture_mouse()
{
	return ImGui::GetIO().WantCaptureMouse;
}

Entity::Type GUI::get_entity_type()
{
	if (mode != Mode::SPAWN)
		return Entity::Type::UNKNOWN;
	return choice;
}

void GUI::show_tools_menu()
{
	ImGui::Begin("Tools");

	if (ImGui::Button("Clear All")) {
		this->clear_all_entity = true;
	}

	ImGui::Text("Mode");

	ImGui::RadioButton("Move", reinterpret_cast<int*>(&this->mode), static_cast<int>(Mode::MOVE_OR_DRAG));
	ImGui::RadioButton("Spawn", reinterpret_cast<int*>(&this->mode), static_cast<int>(Mode::SPAWN));

	//TODO: Add icons to choices
	// You need to implement texture manager to pick that texture from him
	ImGui::BeginDisabled(this->mode != Mode::SPAWN);

	ImGui::Text("Entity");
	ImGui::RadioButton("Rock", (int*)(&this->choice), Entity::Type::ROCK);
	ImGui::RadioButton("Paper", (int*)(&this->choice), Entity::Type::PAPER);
	ImGui::RadioButton("Scissors", (int*)(&this->choice), Entity::Type::SCISSORS);

	ImGui::Text("Quontity mode");
	ImGui::RadioButton("Proportional to area spawn radius", reinterpret_cast<int*>(&this->quantity_mode), static_cast<int>(QuantityMode::PROPORTIONAL));

	ImGui::RadioButton("Linear", reinterpret_cast<int*>(&this->quantity_mode), static_cast<int>(QuantityMode::LINEAR));

	ImGui::BeginDisabled(this->quantity_mode != QuantityMode::LINEAR);
	ImGui::InputInt("Quantity", &this->quantity);

	ImGui::EndDisabled();

	ImGui::EndDisabled();

	ImGui::End();
}
