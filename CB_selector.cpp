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
	c_pick_helper = &c_pick;
	c_pick = patris->getsim()->at_pos(patris->win->mapPixelToCoords({ev.mouseButton.x,ev.mouseButton.y}));
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

void CB_selector::pop_body()
{
	if(c_pick_helper) if(c_pick!=patris->getsim()->get_end()) c_pick = patris->getsim()->erase_body(c_pick);
}

CB_selector::CB_selector()
{
	c_pick_helper = NULL;
}

