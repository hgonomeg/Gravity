#include "Gravity.hpp"


const std::string CB_gen::tool_name = "Celestial body generator";

const std::string& CB_gen::name()
{
	return tool_name;
}

void CB_gen::cycle_type() 
{
	switch(currently_picked)
	{
		case cb_type::Planet:
		{
			currently_picked = cb_type::Star;
			break;
		}
		case cb_type::Star:
		{
			currently_picked = cb_type::StillStar;
			break;
		}
		case cb_type::Asteroid:
		{
			currently_picked = cb_type::Planet;
			break;
		}
		case cb_type::StillStar:
		{
			currently_picked = cb_type::Asteroid;
			break;
		}
		
	}
}

bool CB_gen::mouse_button_pressed(sf::Event::MouseButtonEvent& ev)
{
	if(!b_mode.mouse_button_pressed(ev))
	{
		if(ev.button==sf::Mouse::Button::Left)
		{
			if(!active_state) 
				active_state = true;
			rel_init_pos = main_window->mapPixelToCoords({ev.x,ev.y});
			return true;
		}
	}
	else //if the button got activated
	{
		cycle_type();
	}
	return true;
}
void CB_gen::text_entered(sf::Event::TextEvent& ev)
{

}
void CB_gen::mouse_button_released(sf::Event::MouseButtonEvent& ev)
{
	if(ev.button==sf::Mouse::Button::Left)
	{
		if(active_state) 
		{
			active_state = false;
			rel_end_pos = main_window->mapPixelToCoords({ev.x,ev.y});
			add_body();
		}
	}
}
void CB_gen::keyboard_button_pressed(sf::Event::KeyEvent& ev)
{
	switch(ev.code)
	{
		case sf::Keyboard::M:
		{
			cycle_type();
			break;
		}
		case sf::Keyboard::B:
		{
			if(current_mass > 0) 
				current_mass *= (1.f/1.2f);
			else if(!parent -> debug) 
				current_mass = 1;
			else if(current_mass > -6) 
				current_mass =- 6;
			else 
				current_mass *= 1.2f;
			break;
		}
		case sf::Keyboard::N:
		{
			if(current_mass>0) 
				current_mass *= (1.2f);
			else 
				current_mass *= (1.f/1.2f);
			if(current_mass<5 && current_mass>=0) 
				current_mass++;
			break;
		}
		case sf::Keyboard::H:
		{
			parent->push_hint_text(UI_state::hint_text("CELESTIAL BODY GENERATOR",25000));
			parent->push_hint_text(UI_state::hint_text("M - switch the type of celestial body to be generated",25000));
			parent->push_hint_text(UI_state::hint_text("B / N - decrease / increase mass",25000));
			parent->push_hint_text(UI_state::hint_text("Z - remove last body",25000));
			break;
		}
		case sf::Keyboard::Z:
		{
			if(last_body != 0 && !was_removed) 
			{
				parent->get_simulator()->erase_body(last_body);
				parent->push_hint_text(UI_state::hint_text("Last body removed",500));
				last_body = 0;
			}
			else
			{
				parent->push_hint_text(UI_state::hint_text("There is no last body to remove!",500));
			}
			break;
		}
	}
}
void CB_gen::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(inscription,st);
	tgt.draw(b_mode,st);
}

void CB_gen::add_body()
{
	Celestial_body* neu;
	sf::Vector2f vel = (rel_init_pos-rel_end_pos)*0.01f;
	switch(currently_picked)
	{
		case cb_type::Planet:
		{
			neu = new ::Planet(current_mass,rel_init_pos,vel);
			break;
		}
		case cb_type::StillStar:
		{
			neu = new ::Star(current_mass,rel_init_pos,vel,true);
			break;
		}
		case cb_type::Star:
		{
			neu = new ::Star(current_mass,rel_init_pos,vel,false);
			break;
		}
		case cb_type::Asteroid:
		{
			neu = new ::Asteroid(rel_init_pos,vel);
			break;
		}
		
	}
	
	last_body = neu->get_id();
	was_removed = false;
	parent->get_simulator()->add_body(neu);
}

CB_gen::CB_gen()
:inscription("NULL",resources->main_font,15),
 b_mode(resources->button_gen_mode)
{
	last_body = 0;
	active_state = false;
	was_removed = false;
	current_mass = 10;
	currently_picked = cb_type::Planet;
	temp_planet = Planet::planetry_classification::rock;
	temp_star = Star::stellar_classification::A; 
}

void CB_gen::tick()
{
	std::stringstream tmp;
	tmp<<"Body type: ";

	switch(currently_picked)
	{
		case cb_type::Planet:
		{
			tmp<<"PLANET";
			break;
		}
		case cb_type::Star:
		{
			tmp<<"STAR";
			break;
		}
		case cb_type::Asteroid:
		{
			tmp<<"ASTEROID";
			break;
		}
		case cb_type::StillStar:
		{
			tmp<<"STILL STAR";
			break;
		}
		
	}
	tmp<<" Mass: "<<current_mass; //do zmian
	inscription.setString(tmp.str());
	int offset = 5;
	b_mode.tick();
	b_mode.setPosition({static_cast<float>(offset),static_cast<float>(main_window->getSize().y - 25)}); offset += 20;
	inscription.setPosition(static_cast<float>(offset),static_cast<float>(main_window->getSize().y - 25));
	
}