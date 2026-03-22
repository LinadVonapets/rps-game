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
}

void GUI::process_events(const std::optional<sf::Event> event)
{
	ImGui::SFML::ProcessEvent(this->window, *event);
}

void GUI::update(sf::Time dt)
{
	ImGui::SFML::Update(this->window, dt);
}

void GUI::display()
{
	ImGui::ShowDemoWindow();
	ImGui::SFML::Render(this->window);
}