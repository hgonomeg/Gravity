#include "Simulator.hpp"

const float Simulator::G = 0.5;
float Simulator::STEPPPING_RATE = 1.f;

void Simulator::tick()
{
	if(!paused)
		{
			auto obrob_grawitacje=[this](Celestial_body* lhs, Celestial_body* rhs){
			
			auto &left_loc=lhs->get_loc();
			auto &left_v=lhs->get_v();
			auto &left_mass=lhs->get_mass();
			
			
			auto &right_loc=rhs->get_loc();
			auto &right_v=rhs->get_v();
			auto &right_mass=rhs->get_mass();
			
			float diff_x=left_loc.x-right_loc.x;
			float diff_y=left_loc.y-right_loc.y;
			
			float odleglosc= sqrt((diff_x)*(diff_x)+(diff_y)*(diff_y));
		
			sf::Vector2f sila_graw_vec={diff_x,diff_y};
			sila_graw_vec*=(G*left_mass*right_mass)/(odleglosc*odleglosc*odleglosc);
			
			left_v-=sila_graw_vec/(float)left_mass*STEPPPING_RATE;
			right_v+=sila_graw_vec/(float)right_mass*STEPPPING_RATE;
			
			
			
			};
		
			for(auto j=ciala.begin(); j!=(--ciala.end()); j++)
			{
				
				auto ekaj=j;
				ekaj++;
				for(auto i=ekaj; i!=ciala.end(); i++)
				{
					obrob_grawitacje(j->get(),i->get());	
				}
				
			
			}
			
			for(auto j=ciala.begin(); j!=ciala.end(); j++)
			{
				auto q=j->get();
				q->get_loc()+=(q->get_v())*STEPPPING_RATE; 
			}
		}
}

void Simulator::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	for(auto& x: ciala)
	{
		if(!paused) x.get()->refresh();
		if(draw_traces) x.get()->draw_trace(tgt,st);
		
	}
	for(auto& x: ciala)
	{
		x.get()->draw(tgt,st);
		
	}
}

void Simulator::add_body(Celestial_body* koles)
{
	ciala.push_back(std::unique_ptr<Celestial_body>(koles));
}

std::list<std::unique_ptr<Celestial_body>>::iterator Simulator::erase_body(const std::list<std::unique_ptr<Celestial_body>>::iterator& el)
{
	return ciala.erase(el);
}

std::list<std::unique_ptr<Celestial_body>>::const_iterator Simulator::get_end()
{
	return ciala.cend();
}

std::list<std::unique_ptr<Celestial_body>>::iterator Simulator::at_pos(const sf::Vector2f& here)
{
	
	for(auto i=ciala.begin(); i!=ciala.end(); i++) //i jest iteratorem listy
	{
		auto w=i->get()->getGlobalBounds();

		if(w.contains(here))
			return i;
		
		
		
	}

	return ciala.end();
	
}

void Simulator::pause(bool rzejak)
{
	paused=rzejak;
}

bool Simulator::pause()
{
	return paused;
}

void Simulator::toggle_traces()
{
	draw_traces=!draw_traces;
}

Simulator::Simulator()
{
	paused = false;
	draw_traces = true;
}
