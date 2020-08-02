#include "ResourceLoader.hpp"

Resource_Manager::Resource_Manager() noexcept
{	
	main_font.loadFromMemory(arimo.data,arimo.size);

	auto button_tex_gen = [this](std::string txt, const sf::Color col, bool blackorwhite) -> sf::Texture {
		sf::RenderTexture render_tex;
		sf::Text sf_text(txt,main_font,12);
		render_tex.create(15,15); //the tile
		sf_text.setOrigin({sf_text.getLocalBounds().width/2.f,sf_text.getLocalBounds().height/2.f});
		sf_text.setPosition(7.5,5); //this is roughly the center of the tile
		sf_text.setFillColor(blackorwhite? sf::Color::White : sf::Color::Black);
		render_tex.clear(col);
		render_tex.draw(sf_text);
		render_tex.display();
		return render_tex.getTexture();
	};
	
	button_CB_gen = button_tex_gen("G",sf::Color::Green,false);
	button_CB_selector = button_tex_gen("S",sf::Color::Green,false);
	button_left = button_tex_gen("<",sf::Color::Red,false);
	button_right = button_tex_gen(">",sf::Color::Red,false);
	button_toggle_traces = button_tex_gen("L",sf::Color::Cyan,false);
	button_cycle_collision = button_tex_gen("R",sf::Color::Cyan,false);
	button_deltraces = button_tex_gen("^L",sf::Color::Yellow,false);
	button_gen_mode = button_tex_gen("M",sf::Color::Red,false);
	button_predtraces = button_tex_gen("T",sf::Color::Cyan,false);
	
	button_accuracy_plus = button_tex_gen("A+",sf::Color(48,48,142),true);
	button_accuracy_minus = button_tex_gen("A-",sf::Color(48,48,142),true);
	button_speed_plus = button_tex_gen("S+",sf::Color(48,48,142),true);
	button_speed_minus = button_tex_gen("S-",sf::Color(48,48,142),true);
	button_debug = button_tex_gen("^[",sf::Color(200,50,200),true);

	configuration = load_configuration();
}


void Resource_Manager::finish_loading()
{
	//empty
	//will load external resources from files in the future
}

file_config Resource_Manager::load_configuration() {
	file_config ret; //default constructor fills it with default values
	if(std::filesystem::exists("Gravity.toml"))
	{
		try{
			auto config = cpptoml::parse_file("Gravity.toml");
			auto window_config = config->get_table("window");
			ret.framerate_limit = window_config->get_as<int>("framerate_limit").value_or(ret.framerate_limit); //read from TOML or get the current default value
			ret.vsync = window_config->get_as<bool>("vsync").value_or(ret.vsync); 
			ret.scroll_multiplier = window_config->get_as<double>("scroll_multiplier").value_or(ret.scroll_multiplier);
			ret.translation_constant = window_config->get_as<double>("translation_constant").value_or(ret.translation_constant);
			ret.view_scale = window_config->get_as<double>("default_view_scale").value_or(ret.view_scale);
			ret.x = window_config->get_as<unsigned int>("default_width").value_or(ret.x);
			ret.y = window_config->get_as<unsigned int>("default_height").value_or(ret.y);
			ret.origin_x = window_config->get_as<double>("origin_x").value_or(ret.origin_x);
			ret.origin_y = window_config->get_as<double>("origin_y").value_or(ret.origin_y);
		}catch(std::exception& e){
			std::cerr<<"Error reading/parsing toml config file \"Gravity.toml\":\n"<<e.what();
			return file_config(); //return defaults
		}
	}else{ //create a config file for future
		auto window_config = cpptoml::make_table();
		window_config->insert("framerate_limit",ret.framerate_limit);
		window_config->insert("vsync",ret.vsync);
		window_config->insert("scroll_multiplier",ret.scroll_multiplier);
		window_config->insert("translation_constant",ret.translation_constant);
		window_config->insert("default_view_scale",ret.view_scale);
		window_config->insert("default_width",ret.x);
		window_config->insert("default_height",ret.y);
		window_config->insert("origin_x",ret.origin_x);
		window_config->insert("origin_y",ret.origin_y);
		auto config = cpptoml::make_table();
		config->insert("window",window_config);

		std::ofstream config_file;
		config_file.open("Gravity.toml",std::ios::out);
		cpptoml::toml_writer writer(config_file);
		config->accept(writer); //this weird API tells the writer to write the 'config' table
		config_file.close();
	}

	return ret;
}

file_config::file_config() {
	vsync = true;
	framerate_limit = 144;
	scroll_multiplier = 1.05;
	translation_constant = 30;
	view_scale = 1;
	x = 960;
	y = 500;
	origin_x = 0;
	origin_y = 0;
}

