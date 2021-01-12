
#ifndef RESOURCE_LOADER_HPP
#include "resources/fonts/Arimo-Regular.hpp"
#include "Space_objects.hpp"
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>
#include <memory>
#include <cpptoml.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <map>

#define RESOURCE_LOADER_HPP

struct file_config {
	bool vsync;
	int framerate_limit;
	float translation_constant, scroll_multiplier, view_scale, origin_x, origin_y;
	unsigned int x, y;
	file_config();
};
class Resource_Manager
{
 public:

	const static std::map<std::string,unsigned short> num_of_external_textures_map; //for each space object (Planet, Star,asteroid) maps number of its textures
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

	file_config configuration;

	Resource_Manager() noexcept; //loads essentials like the font and procedurally generated textures and won't fail
	void finish_loading() const; //may fail if resources are missing
	file_config load_configuration(); //manages config files (loads or creates)

	private:

	void load_planet_textures(std::vector<sf::Texture> vec) const;
	void load_star_textures(std::vector<sf::Texture> vec) const;
	void load_asteroid_textures(std::vector<sf::Texture> vec) const;
	
};

inline std::shared_ptr<Resource_Manager> resources;
inline std::shared_ptr<sf::RenderWindow> main_window;



#endif