#include "ResourceLoader.hpp"

ResourceBase* LoadResources()
{
	ResourceBase* neter = new ResourceBase;
	neter->except_text="All correct.";
	neter->correct=true;
	
	auto button_tex_gen = [](std::string txt, const sf::Color& col, bool blackorwhite)->sf::Texture{
		sf::RenderTexture ren;
		sf::Text textext(txt,*fona,12);
		ren.create(15,15);
		textext.setOrigin({textext.getLocalBounds().width/2.f,textext.getLocalBounds().height/2.f});
		textext.setPosition(7.5,5);
		textext.setFillColor(blackorwhite? sf::Color::White : sf::Color::Black);
		ren.clear(col);
		ren.draw(textext);
		ren.display();
		return ren.getTexture();
	};
	
	neter->button_CB_gen = button_tex_gen("G",sf::Color::Green,false);
	neter->button_CB_selector = button_tex_gen("S",sf::Color::Green,false);
	neter->button_left = button_tex_gen("<",sf::Color::Red,false);
	neter->button_right = button_tex_gen(">",sf::Color::Red,false);
	neter->button_toggle_traces = button_tex_gen("L",sf::Color::Cyan,false);
	neter->button_cycle_collision = button_tex_gen("R",sf::Color::Cyan,false);
	neter->button_deltraces = button_tex_gen("^L",sf::Color::Yellow,false);
	neter->button_gen_mode = button_tex_gen("M",sf::Color::Red,false);
	neter->button_predtraces = button_tex_gen("T",sf::Color::Cyan,false);
	
	neter->button_accuracy_plus = button_tex_gen("A+",sf::Color(48,48,142),true);
	neter->button_accuracy_minus = button_tex_gen("A-",sf::Color(48,48,142),true);
	neter->button_speed_plus = button_tex_gen("S+",sf::Color(48,48,142),true);
	neter->button_speed_minus = button_tex_gen("S-",sf::Color(48,48,142),true);
	neter->button_debug = button_tex_gen("^[",sf::Color(200,50,200),true);
	return neter;
}