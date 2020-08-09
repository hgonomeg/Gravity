
#ifndef RESOURCE_LOADER_HPP
#include "resources/Arimo-Regular.hpp"
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>
#include <memory>

#define RESOURCE_LOADER_HPP

class Resource_Manager
{
 public:
	
	const int ui_font_size; //does not include buttons
	sf::Font main_font;
	
	//procedural textures
	sf::Texture button_CB_gen;
	sf::Texture button_CB_selector;
	sf::Texture button_right;
	sf::Texture button_left;
	sf::Texture button_toggle_traces;
	sf::Texture button_cycle_collision;
	sf::Texture button_deltraces;
	sf::Texture button_predtraces;
	sf::Texture button_gen_mode;
	sf::Texture button_accuracy_plus;
	sf::Texture button_accuracy_minus;
	sf::Texture button_speed_plus;
	sf::Texture button_speed_minus;
	sf::Texture button_debug;

	Resource_Manager() noexcept; //loads essentials like the font and procedurally generated textures and won't fail
	void finish_loading(); //may fail if resources are missing
	
};

inline std::shared_ptr<Resource_Manager> resources;
inline std::shared_ptr<sf::RenderWindow> main_window;



#endif