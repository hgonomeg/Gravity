#include "Gravity.hpp"

const std::string CB_selector::tool_name="Celestial body selector";
const std::string& CB_selector::name()
{
	return tool_name;
}

bool CB_selector::mouse_button_pressed(sf::Event::MouseButtonEvent& ev)
{
	if(ev.button==sf::Mouse::Button::Left)
	{
		current_pick_iter = parent->get_simulator()->at_pos(main_window->mapPixelToCoords({ev.x,ev.y}));

		if(current_pick_iter!=parent->get_simulator()->get_end())	
		{
			current_pick =  current_pick_iter->get();
			pick_id = current_pick->get_id();
		}
		else 
			current_pick = nullptr;

		return true;
	}
	return false;
}
void CB_selector::text_entered(sf::Event::TextEvent& ev)
{
	
}
void CB_selector::mouse_button_released(sf::Event::MouseButtonEvent& ev)
{
	
}

void CB_selector::keyboard_button_pressed(sf::Event::KeyEvent& ev)
{
	switch(ev.code)
	{
		case sf::Keyboard::E:
		{
			parent->push_hint_text(UI_state::hint_text("Editing currently not supported",500));
			break;
		}
		case sf::Keyboard::X:
		{
			pop_body();
			break;
		}
		case sf::Keyboard::H:
		{
			parent->push_hint_text(UI_state::hint_text("CELESTIAL BODY SELECTOR",25000));
			parent->push_hint_text(UI_state::hint_text("E - edit the currently selected body",25000));
			parent->push_hint_text(UI_state::hint_text("X - remove the currently selected body",25000));
			break;
		}
	}
}
void CB_selector::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(inscription);
}

bool CB_selector::verify_body()
{
	if(Celestial_body::get_alloc_diagram().at(pick_id) == pick_id) //if the id of the pick still exists 
	{
		if(current_pick) 
		{
			return true;
		}
		else 
			return false;
	}
	else //else get to the body that was produced as a result of a collision with the original body
	{
		pick_id = Celestial_body::get_alloc_diagram().at(pick_id);
		current_pick_iter = parent->get_simulator()->iterator_of(pick_id);

		if(current_pick_iter != parent->get_simulator()->get_end())	
		{
			current_pick =  current_pick_iter->get();
		}
		else 
			current_pick = nullptr;
		
		return verify_body();
	}
}

void CB_selector::pop_body()
{
	if(verify_body())
	{
		current_pick_iter = parent->get_simulator()->erase_body(current_pick_iter);
		if(current_pick_iter!=parent->get_simulator()->get_end())	
		{
			current_pick =  current_pick_iter->get();
			pick_id = current_pick->get_id();
		}
		else 
			current_pick = nullptr;
	}
	else 
		parent->push_hint_text(UI_state::hint_text("Nothing selected!",500));
}

CB_selector::CB_selector()
:inscription("Current selection: ",resources->main_font,15)
{
	current_pick = nullptr;
	pick_id = 0;
}

void CB_selector::tick()
{
	std::stringstream tmp;
	tmp<<"Current selection: ";
	if(verify_body())
	{
		tmp<<"ID: "<<current_pick->get_id()<<" Type: ";

		if(dynamic_cast<Planet*>(current_pick) != nullptr) 
			tmp<<"PLANET";
		if(dynamic_cast<Star*>(current_pick) != nullptr) 
			tmp<<"STAR";

		tmp<<" Mass: "<<current_pick->get_mass();
	}
	else tmp<<"None";
	inscription.setString(tmp.str());
	inscription.setPosition(5,main_window->getSize().y - 25);
}

