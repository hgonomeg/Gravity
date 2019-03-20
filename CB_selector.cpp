#include "Grawitacja.hpp"

const std::string CB_selector::nam="Celestial body selector";
const std::string& CB_selector::name()
{
	return nam;
}

bool CB_selector::mbp(sf::Event& ev)
{
	if(ev.mouseButton.button==sf::Mouse::Button::Left)
	{
	c_pick_iter = patris->getsim()->at_pos(win->mapPixelToCoords({ev.mouseButton.x,ev.mouseButton.y}));
	if(c_pick_iter!=patris->getsim()->get_end())	
		{
		c_pick =  c_pick_iter->get();
		pick_id = c_pick->get_id();
		}
	else c_pick = NULL;
	return true;
	}
	return false;
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
		case sf::Keyboard::H:
		{
			patris->push_hint_text(UI_state::hint_text("CELESTIAL BODY SELECTOR",25000));
			patris->push_hint_text(UI_state::hint_text("E - edit the currently selected body",25000));
			patris->push_hint_text(UI_state::hint_text("X - remove the currently selected body",25000));
			break;
		}
	}
}
void CB_selector::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(napis);
}

bool CB_selector::verify_body()
{
	if(Celestial_body::get_alloc_diagram().at(pick_id)==pick_id)
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
		if(c_pick_iter!=patris->getsim()->get_end())	
		{
		c_pick =  c_pick_iter->get();
		pick_id = c_pick->get_id();
		}
		else c_pick = NULL;
	}
	else patris->push_hint_text(UI_state::hint_text("Nothing selected!",500));
}

CB_selector::CB_selector()
:napis("Current selection: ",*fona,15)
{
	c_pick = NULL;
	pick_id = 0;
}

void CB_selector::tick()
{
	std::string tmp = "Current selection: ";
	if(verify_body())
	{
		tmp+="ID: "+std::to_string(c_pick->get_id())+" Type: ";
		if(dynamic_cast<Planet*>(c_pick)!=NULL) tmp+="PLANET";
		if(dynamic_cast<Star*>(c_pick)!=NULL) tmp+="STAR";
		tmp+=" Mass: "+std::to_string(c_pick->get_mass());
	}
	else tmp+="NULL";
	napis.setString(tmp);
	napis.setPosition(5,patris->gettgt()->getSize().y-25);
}

