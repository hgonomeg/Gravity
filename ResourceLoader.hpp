
#ifndef RESOURCE_LOADER_HPP
#include "resources/Arimo-Regular.hpp"
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>

#define RESOURCE_LOADER_HPP

struct ResourceBase
{
	std::string except_text;
	sf::Texture button_CB_gen;
	sf::Texture button_CB_selector;
	sf::Texture button_right;
	sf::Texture button_left;
	sf::Texture button_toggle_traces;
	sf::Texture button_cycle_collision;
	bool correct;
};

ResourceBase* LoadResources();

#if defined(EXTERNPLEASE)
extern ResourceBase* zasoby;
extern sf::Font* fona;
extern sf::RenderWindow* win;
extern sf::Vector2u* whatsize;
#else
sf::Font* fona;
ResourceBase* zasoby;
sf::RenderWindow* win;
sf::Vector2u* whatsize;
#endif

#endif