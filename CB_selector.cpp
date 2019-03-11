#include "Grawitacja.hpp"

const std::string CB_selector::nam="Celestial body selector";
const std::string& CB_selector::name()
{
	return nam;
}

void CB_selector::mbp(sf::Event& ev)
{
	if(ev.mouseButton.button==sf::Mouse::Button::Left)
	{
	c_pick_iter = patris->getsim()->at_pos(patris->win->mapPixelToCoords({ev.mouseButton.x,ev.mouseButton.y}));
	c_pick =  c_pick_iter->get();
	pick_id = c_pick->get_id();
	}
}
void CB_selector::mbr(sf::Event& ev)
{
	
}
void CB_selector::kbp(sf::Event& ev)
{
	switch(ev.key.code)
	{
		case sf::Keyboard::E:
		{
			patris->push_hint_text(UI_state::hint_text("Editing currently not supported",500));
			break;
		}
		case sf::Keyboard::X:
		{
			pop_body();
			break;
		}
	}
}
void CB_selector::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	
}

bool CB_selector::verify_body()
{
	if(Celestial_body::get_alloc_diagram()[pick_id])
	{
		if(c_pick)
		{
			return true;
		}
		else return false;
	}
	else return false;
}

void CB_selector::pop_body()
{
	if(verify_body())
	{
		c_pick_iter = patris->getsim()->erase_body(c_pick_iter);
		c_pick =  c_pick_iter->get();
		pick_id = c_pick->get_id();
	}
}

CB_selector::CB_selector()
{
	c_pick = NULL;
	pick_id = 0;
}

