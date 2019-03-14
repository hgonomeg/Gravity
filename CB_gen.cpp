#include "Grawitacja.hpp"
#include "Textbox.hpp"

const std::string CB_gen::nam="Celestial body generator";

const std::string& CB_gen::name()
{
	return nam;
}

void CB_gen::mbp(sf::Event& ev)
{
	if(ev.mouseButton.button==sf::Mouse::Button::Left)
	{
	if(!active_state) active_state = true;
	rel_init = win->mapPixelToCoords({ev.mouseButton.x,ev.mouseButton.y});
	}
}
void CB_gen::mbr(sf::Event& ev)
{
	if(ev.mouseButton.button==sf::Mouse::Button::Left)
	{
	if(active_state) active_state = false;
	rel_end = win->mapPixelToCoords({ev.mouseButton.x,ev.mouseButton.y});
	add_body();
	}
}
void CB_gen::kbp(sf::Event& ev)
{
	switch(ev.key.code)
	{
		case sf::Keyboard::M:
		{
			patris->push_hint_text(UI_state::hint_text("Mode switching currently not supported",500));
			break;
		}
	}
}
void CB_gen::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	
}

void CB_gen::add_body()
{
	Celestial_body* neu; //do zmian
	neu = new ::Planet(current_mass,rel_init,(rel_init-rel_end)*0.05f);
	patris->getsim()->add_body(neu);
}

CB_gen::CB_gen()
{
	active_state = false;
	current_mass=10;
	currently_picked = cb_type::Planet;
	temp_planet = Planet::planetary_classification::rock;
	temp_star = Star::stellar_classification::A; 
}

void CB_gen::tick()
{
	
}