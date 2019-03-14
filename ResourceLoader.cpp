#include "ResourceLoader.hpp"

ResourceBase* LoadResources()
{
	ResourceBase* neter = new ResourceBase;
	neter->except_text="All correct.";
	neter->correct=true;
	
	auto button_tex_gen = [](std::string txt, sf::Color col)->sf::Texture{
		sf::RenderTexture ren;
		sf::Text textext(txt,*fona,12);
		ren.create(15,15);
		textext.setPosition(3,3);
		textext.setFillColor(sf::Color::Black);
		ren.clear(col);
		ren.draw(textext);
		ren.display();
		return ren.getTexture();
	};
	
	neter->button_CB_gen = button_tex_gen("G",sf::Color::Green);
	neter->button_CB_selector = button_tex_gen("S",sf::Color::Green);
	neter->button_left = button_tex_gen("<",sf::Color::Red);
	neter->button_right = button_tex_gen(">",sf::Color::Red);
	
	return neter;
}