#include "GUI.hpp"

#include <iostream>

#include <imgui.h>
#include <imgui-SFML.h>

GUI::GUI(sf::RenderWindow& window)
	: window{window}
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
	this->spawn_area.setOutlineThickness(1.f);
	this->spawn_area.setOutlineColor(sf::Color::Red);
	
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
		if (mouseWheelScrolled->delta < 0)
			spawn_area_radius += 10;
		else
			spawn_area_radius -= 10;

		if (spawn_area_radius < 0)
			spawn_area_radius = 0;
		else 
		if (spawn_area_radius > 300)
			spawn_area_radius = 300;
		
		this->spawn_area.setRadius(spawn_area_radius);
		this->spawn_area.setOrigin(this->spawn_area.getLocalBounds().getCenter());
		
	}
}

void GUI::update(sf::Time dt)
{
	ImGui::SFML::Update(this->window, dt);
	this->spawn_area.setPosition(sf::Vector2f(sf::Mouse::getPosition(this->window)));
}

void GUI::display()
{
	if(this->show)
	{
		ImGui::Begin("Tools");
		ImGui::Text("Mode");

		ImGui::RadioButton("Move", reinterpret_cast<int*>(&mode), static_cast<int>(Mode::MOVE_OR_DRAG));
		ImGui::RadioButton("Spawn", reinterpret_cast<int*>(&mode), static_cast<int>(Mode::SPAWN));
		

		//TODO: Add icons to choices
		// You need to implement texture manager to pick that texture from him
		ImGui::Text("Entity:");

		ImGui::BeginDisabled(mode != Mode::SPAWN);

		ImGui::RadioButton("Rock", (int*)(&choice), Entity::Type::ROCK);
		ImGui::RadioButton("Paper", (int*)(&choice), Entity::Type::PAPER);
		ImGui::RadioButton("Scissors", (int*)(&choice), Entity::Type::SCISSORS);
		
		ImGui::EndDisabled();

		ImGui::End();
	}
	
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
