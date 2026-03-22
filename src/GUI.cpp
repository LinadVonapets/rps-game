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
	choice = {.rock = true};
}

void GUI::process_events(const std::optional<sf::Event> event)
{
	ImGui::SFML::ProcessEvent(this->window, *event);
	if (const auto* key = event->getIf<sf::Event::KeyPressed>()) 
	{	
		if (key->scancode == sf::Keyboard::Scancode::M)
			this->show =! this->show;
	}
}

void GUI::update(sf::Time dt)
{
	ImGui::SFML::Update(this->window, dt);
}

void GUI::display()
{
	if(this->show)
	{
		ImGui::Begin("Pickup Entity");
		if (ImGui::RadioButton("Rock", choice.rock))
			choice = {.rock=true};

		if (ImGui::RadioButton("Paper", choice.paper))
			choice = {.paper=true};
		
		if (ImGui::RadioButton("Scissors", choice.scissors))
			choice = {.scissors=true};
		
		ImGui::End();
	}

	ImGui::SFML::Render(this->window);
}

bool GUI::is_want_capture_mouse() 
{
	return ImGui::GetIO().WantCaptureMouse;
}

Entity::Type GUI::get_entity_type() 
{
	if (choice.rock)
		return Entity::Type::ROCK;
	else if (choice.paper)
		return Entity::Type::PAPER;
	else if (choice.scissors)
		return Entity::Type::SCISSORS;
}