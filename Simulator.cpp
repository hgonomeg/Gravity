#include "Simulator.hpp"
#include <list>

const float Simulator::G = 0.05;
float Simulator::STEPPPING_RATE = 1.f;
const unsigned short Simulator::CA_count = 2;

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
					if(Celestial_body::collision_detec(j->get(),i->get()))
					{
						switch(ca)
							{
							case collision_approach::merge:
								{
								//i jako ojciec, jest zawsze nadpisywane dzieckiem. j usuwamy samemu
								Celestial_body* ojc=i->release();
								Celestial_body::collision_handle(j->get(),ojc);	
								i->reset(ojc);
								ciala.erase(j); 
								j=i; j--;
								break;
								}
							case collision_approach::bounce:
								{
								Celestial_body::bounce_handle(j->get(),i->get());
								break;
								}
							}
						if(i==ciala.begin()) break;
					}
				}
				
			
			}
			
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

void Simulator::erase_body(unsigned int ajdi)
{
	for(auto i=ciala.begin(); i!=ciala.end(); i++) if(i->get()->get_id()==ajdi) {erase_body(i); return;}
}

std::list<std::unique_ptr<Celestial_body>>::iterator Simulator::erase_body(const std::list<std::unique_ptr<Celestial_body>>::iterator& el)
{
	return ciala.erase(el);
}

std::list<std::unique_ptr<Celestial_body>>::const_iterator Simulator::get_end()
{
	return ciala.cend();
}

std::list<std::unique_ptr<Celestial_body>>::iterator Simulator::iterator_of(unsigned int ajdi)
{
	for(auto i=ciala.begin();i!=ciala.end();i++)
	{
		if(i->get()->get_id()==ajdi) return i;
	}
	return ciala.end();
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

void Simulator::delete_traces()
{
	for(auto& x: ciala) x->delete_traces();
}

Simulator::collision_approach Simulator::cycle_collision_approach()
{
	unsigned short u = (unsigned short)ca;
	u++;
	if(u>CA_count) u=1;
	ca = (collision_approach)u;
	return ca;
}

Simulator::Simulator()  //kostruktor domyślny
{
	paused = false;
	draw_traces = true;
	ca = collision_approach::merge;
}

Simulator::Simulator(const Simulator &sim) //kostruktor kopiujący
{
	paused=sim.paused;
	draw_traces=sim.draw_traces;
	ca=sim.ca;
	
	//ciala
	
	
	
	for(auto i=sim.ciala.begin(); i!=sim.ciala.end(); i++)
	{
		Celestial_body* bufor = NULL;
		
		//Leszek czy tutaj podrodze nie trzeba użyć "new" na buforze?
		
		//biore wskażnik ze starej listy
		
		Celestial_body* wsk_stare_cialo = i->get(); //pobranie tego na co wskazuje "i"
		
		bufor = wsk_stare_cialo->clone(*wsk_stare_cialo); //objekt stare ciało się kopuje które ma być w buforze 
		
		ciala.push_back(std::unique_ptr<Celestial_body>(bufor)); //push do docelowej listy

	}
	
	
	
}