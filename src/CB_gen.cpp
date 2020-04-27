#include "Grawitacja.hpp"
#include "Textbox.hpp"

const std::string CB_gen::nam="Celestial body generator";

const std::string& CB_gen::name()
{
	return nam;
}

bool CB_gen::mbp(sf::Event& ev)
{
	if(!b_mode.mbp(ev))
	{
		if(ev.mouseButton.button==sf::Mouse::Button::Left)
		{
			if(!active_state) active_state = true;
			rel_init = patris->gettgt()->mapPixelToCoords({ev.mouseButton.x,ev.mouseButton.y});
			return true;
		}
	}
	else 
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
	return true;
}
void CB_gen::mbr(sf::Event& ev)
{
	if(ev.mouseButton.button==sf::Mouse::Button::Left)
	{
	if(active_state) 
		{
		active_state = false;
		rel_end = patris->gettgt()->mapPixelToCoords({ev.mouseButton.x,ev.mouseButton.y});
		add_body();
		}
	}
}
void CB_gen::kbp(sf::Event& ev)
{
	switch(ev.key.code)
	{
		case sf::Keyboard::M:
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
			break;
		}
		case sf::Keyboard::B:
		{
			if(current_mass>0)current_mass*=(1.f/1.2f);
			else if(!patris->debug) current_mass=1;
				else if(current_mass>-6) current_mass=-6;
					else current_mass*=1.2f;
			break;
		}
		case sf::Keyboard::N:
		{
			if(current_mass>0) current_mass*=(1.2f);
			else current_mass*=(1.f/1.2f);
			if(current_mass<5&&current_mass>=0) current_mass++;
			break;
		}
		case sf::Keyboard::H:
		{
			patris->push_hint_text(UI_state::hint_text("CELESTIAL BODY GENERATOR",25000));
			patris->push_hint_text(UI_state::hint_text("M - switch the type of celestial body to be generated",25000));
			patris->push_hint_text(UI_state::hint_text("B / N - decrease / increase mass",25000));
			patris->push_hint_text(UI_state::hint_text("Z - remove last body",25000));
			break;
		}
		case sf::Keyboard::Z:
		{
			if(lbod&&!didrem) 
			{
			patris->getsim()->erase_body(lbod->get_id());
			patris->push_hint_text(UI_state::hint_text("Last body removed",500));
			lbod=NULL;
			}
			else
			{
				patris->push_hint_text(UI_state::hint_text("There is no last body to remove!",500));
			}
			break;
		}
	}
}
void CB_gen::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(napis,st);
	tgt.draw(b_mode,st);
}

void CB_gen::add_body()
{
	Celestial_body* neu;
	sf::Vector2f vel = (rel_init-rel_end)*0.01f;
	switch(currently_picked)
	{
		case cb_type::Planet:
		{
			neu = new ::Planet(current_mass,rel_init,vel);
			break;
		}
		case cb_type::StillStar:
		case cb_type::Star:
		{
			neu = new ::Star(current_mass,rel_init,vel,wizja);
			break;
		}
		case cb_type::Asteroid:
		{
			neu = new ::Asteroid(rel_init,vel);
			break;
		}
		
	}
	
	lbod = neu;
	didrem=false;
	patris->getsim()->add_body(neu);
}

CB_gen::CB_gen()
:napis("NULL",resources->main_font,15),
 b_mode(resources->button_gen_mode)
{
	lbod = NULL;
	active_state = false;
	wizja = true;
	didrem=false;
	current_mass=10;
	currently_picked = cb_type::Planet;
	temp_planet = Planet::planetary_classification::rock;
	temp_star = Star::stellar_classification::A; 
}

void CB_gen::tick()
{
	std::string tmp="Body type: ";
	switch(currently_picked)
	{
		case cb_type::Planet:
		{
			tmp+="PLANET";
			break;
		}
		case cb_type::Star:
		{
			tmp+="STAR";
			wizja=false;
			break;
		}
		case cb_type::Asteroid:
		{
			tmp+="ASTEROID";
			break;
		}
		case cb_type::StillStar:
		{
			tmp+="STILL STAR";
			wizja=true;
			break;
		}
		
	}
	tmp+=" Mass: "+std::to_string(current_mass); //do zmian
	napis.setString(tmp);
	int renlen=5;
	b_mode.tick();
	b_mode.setPosition({static_cast<float>(renlen),static_cast<float>(patris->gettgt()->getSize().y-25)}); renlen+=20;
	napis.setPosition(static_cast<float>(renlen),static_cast<float>(patris->gettgt()->getSize().y-25));
	
}