#include "Simulator.hpp"

const float Simulator::G = 0.05;
float Simulator::STEPPING_RATE = 1.f;
const unsigned short Simulator::CA_count = 2;
int Simulator::accuracy_factor = 0;
unsigned int Simulator::tick_rate = 1;

void Simulator::change_accuracy(bool chg)
{
	if(chg) accuracy_factor++; else accuracy_factor--;
	if(accuracy_factor==0) STEPPING_RATE=1.f;
	else
	{
		if(accuracy_factor<0) STEPPING_RATE=fabs(static_cast<float>(accuracy_factor));
		else STEPPING_RATE=1/static_cast<float>(accuracy_factor);
	}
}

bool Simulator::change_rate(bool chg)
{
	if(chg)
	{
		tick_rate++;
	}
	else
	{
		if(tick_rate==1) return false;
		tick_rate--;
	}
	return true;
}

float Simulator::get_accuracy()
{
	return 1.f/STEPPING_RATE;
}
unsigned int Simulator::get_rate()
{
	return tick_rate;
}

std::size_t Simulator::size()
{
	return ciala.size();
}

void Simulator::tick()
{
	if(!paused)
		{
			if(predicted_traces)
			{
				for(auto u=predicted_traces->begin();u!=predicted_traces->end();u++)
				{
					for(auto i=u->begin();i!=u->end();i++)
					{
						if(i->color.a>0)
						{
							i->color.a--;
						}
					}
					if(u->back().color.a==0) 
						{
						u=predicted_traces->erase(u); 
						if(u==predicted_traces->end()) break;
						}
				}
				if(predicted_traces->size()==0) {delete predicted_traces; predicted_traces = NULL; }
			}
			for(unsigned uk=0;uk<tick_rate;uk++)
			{
				auto obrob_grawitacje=[this](Celestial_body* lhs, Celestial_body* rhs) {
				
					
					lhs->simultaneity_guardian.lock();
					auto left_loc=lhs->get_location();
					auto left_v=lhs->get_velocity();
					auto left_mass=lhs->get_mass();
					lhs->simultaneity_guardian.unlock();
					
					rhs->simultaneity_guardian.lock();
					auto right_loc=rhs->get_location();
					auto right_v=rhs->get_velocity();
					auto right_mass=rhs->get_mass();
					rhs->simultaneity_guardian.unlock();

					float diff_x=left_loc.x-right_loc.x;
					float diff_y=left_loc.y-right_loc.y;
					
					float odleglosc = sqrt((diff_x)*(diff_x)+(diff_y)*(diff_y));
				
					sf::Vector2f sila_graw_vec={diff_x,diff_y};
					if(odleglosc!=0) 
						sila_graw_vec*=(G*left_mass*right_mass)/(odleglosc*odleglosc*odleglosc);
					else
						sila_graw_vec= {0,0};
					lhs->simultaneity_guardian.lock();
					lhs->set_velocity(left_v-sila_graw_vec/(float)left_mass*STEPPING_RATE);
					lhs->simultaneity_guardian.unlock();

					rhs->simultaneity_guardian.lock();
					rhs->set_velocity(right_v+sila_graw_vec/(float)right_mass*STEPPING_RATE);
					rhs->simultaneity_guardian.unlock();
					
				};
				
				twx.async_pairwise_apply([obrob_grawitacje](const std::list<std::unique_ptr<Celestial_body>>::iterator& ein,const std::list<std::unique_ptr<Celestial_body>>::iterator& zwei) {
					obrob_grawitacje(ein->get(),zwei->get());
				});


				auto detect_collisions = [](std::vector<std::pair<unsigned int,unsigned int>>& detected, std::mutex& detected_mutex,const std::list<std::unique_ptr<Celestial_body>>::iterator lhs,const std::list<std::unique_ptr<Celestial_body>>::iterator rhs)
				{
					auto* lhs_ptr = lhs->get();
					auto* rhs_ptr = rhs->get();
					if(Celestial_body::collision_detection(*lhs_ptr,*rhs_ptr))
					{
						std::lock_guard<std::mutex> my_lock(detected_mutex);
						{
							detected.push_back({lhs_ptr->get_id(),rhs_ptr->get_id()});
						}
					}
					
				};


				std::vector<std::pair<unsigned int,unsigned int>> detected_pairs;
				std::mutex detected_mutex;
				std::set<unsigned int> deleted_bodies;
				

				twx.async_pairwise_apply(std::bind(detect_collisions,std::ref(detected_pairs),std::ref(detected_mutex),std::placeholders::_1,std::placeholders::_2));
				
				for(auto& x: detected_pairs) 
				{
					if(deleted_bodies.find(x.first)==deleted_bodies.end() && deleted_bodies.find(x.second)==deleted_bodies.end())
					{
						switch(ca)
						{
						case collision_approach::merge:
							{
							//first jako ojciec, jest zawsze nadpisywane dzieckiem. second usuwamy samemu
							auto father_iterator = iterator_of(x.first);
							auto mother_iterator = iterator_of(x.second);
							Celestial_body* father = father_iterator->release();
							deleted_bodies.insert(x.first); //mark the father as a deleted body because it will be overwritten
							deleted_bodies.insert(x.second);//this one gets deleted too
							Celestial_body::collision_handle(mother_iterator->get(),father); //father is being overwritten (becomes child)
							father_iterator->reset(father); //father is now the child
							
							ciala.erase(mother_iterator); //I suppose a possible memory leak when the same body collides with more bodies
							break;
							}
						case collision_approach::bounce:
							{
							Celestial_body::bounce_handle(iterator_of(x.second)->get(),iterator_of(x.first)->get());
							break;
							}
						}
					}
				}

				//set velocity
				for(auto j=ciala.begin(); j!=ciala.end(); j++)
				{
					auto q=j->get();
					q->set_location(q->get_location()+(q->get_velocity())*STEPPING_RATE); 
				}
			}
		}
}

void Simulator::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	for(auto& x: ciala)
	{
		if(!paused) 
			x->refresh(); //refresh the state of the graphical part of the object
		if(draw_traces) 
			x->draw_trace(tgt,st);
		
	}
	for(auto& x: ciala) //has to be drawn in a separate loop no to overlap with the traces
	{
		x->draw(tgt,st);
	}
	if(predicted_traces)
	{
		for(auto& x: (*predicted_traces)) tgt.draw(&x[0],x.size(),sf::LineStrip,st);
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

std::list<std::vector<sf::Vertex>> Simulator::get_traces()
{
	std::list<std::vector<sf::Vertex>> ret;
	for(auto& x: ciala) ret.splice(ret.begin(),x->get_traces());
	return ret;
}

void Simulator::predict_traces()
{
	Simulator przodnik(*this);
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
:ciala(),
twx(ciala)
{
	Celestial_body::pushstax();
	paused = false;
	draw_traces = true;
	ca = collision_approach::merge;
	predicted_traces=NULL;
}

Simulator::Simulator(const Simulator &sim) //kostruktor kopiujący
:ciala(),
twx(ciala)
{
	Celestial_body::pushstax();
	paused=sim.paused;
	draw_traces=sim.draw_traces;
	ca=sim.ca;
	predicted_traces=NULL;
	//ciala.clear();
	//ciala
	
	
	
	for(auto i=sim.ciala.begin(); i!=sim.ciala.end(); i++)
	{
		Celestial_body* bufor = NULL;
		
		Celestial_body* wsk_stare_cialo = i->get(); //pobranie wskaźnika do Celestial_body którym opiekuje się obiekt unique_ptr znajdujący się pod iteratorem "i".
		
		bufor = wsk_stare_cialo->clone(*wsk_stare_cialo); //objekt stare ciało się kopuje które ma być w buforze 
		
		ciala.push_back(std::unique_ptr<Celestial_body>(bufor)); //push do docelowej listy

	}
	
	
	
}

Simulator::~Simulator()
{
	Celestial_body::popstax();
}


