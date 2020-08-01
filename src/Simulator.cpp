#include "Simulator.hpp"

const float Simulator::G = 0.05;
float Simulator::STEPPING_RATE = 1.f;
const unsigned short Simulator::CA_count = 3;
const float Simulator::overlap_tolerance = 0.1; //bodies can overlap up to 10%
int Simulator::accuracy_factor = 0;
unsigned int Simulator::tick_rate = 1;

void Simulator::change_accuracy(bool chg)
{
	//change accuracy variable accordingly
	if(chg) 
		accuracy_factor++; 
	else 
		accuracy_factor--;
	
	//accuracy_factor equal to zero means default values
	if(accuracy_factor==0) 
		STEPPING_RATE=1.f;
	else
	{
		if(accuracy_factor<0) //accuracy below zero requires increasing the stepping rate, value of which reflects the accuracy parameter 
			STEPPING_RATE = fabs(static_cast<float>(accuracy_factor));
		else //accuracy above zero means that we have to set the stepping rate below 1, in a hyperbolic way to reflect accuracy
			STEPPING_RATE = 1/static_cast<float>(accuracy_factor+1);
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
float Simulator::get_overlap_tolerance()
{
	return overlap_tolerance;
}

unsigned int Simulator::get_rate()
{
	return tick_rate;
}

void Simulator::set_circle_approx_polygon(unsigned int sides)
{
	Celestial_body::set_global_num_of_polygon_sides(sides);
	for(auto& x: body_list)
	{
		x->set_num_of_polygon_sides(sides);
	}
}

std::size_t Simulator::size() const
{
	return body_list.size();
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
					if(i->color.a>0) //dim predicted traces to full transparency
					{
						i->color.a--;
					}
				}
				if(u->back().color.a==0) //when the last node of a trace is transparent, the whole trace can be disposed of
				{
					u = predicted_traces->erase(u); 
					if(u == predicted_traces->end()) 
						break;
				}
			}
			if(predicted_traces->size()==0) 
			{
				delete predicted_traces; 
				predicted_traces = nullptr; 
			}
		}
		for(unsigned internal_cycles=0; internal_cycles<tick_rate; internal_cycles++)
		{
			//computes gravity force and applies its' physical effects to targets 
			//THREAD-SAFE
			auto compute_gravity = [this](Celestial_body* lhs, Celestial_body* rhs) {
				if(lhs == nullptr || rhs == nullptr)
					throw std::runtime_error("Tried to compute gravity with a null-pointer.");
				
				//get data in a thread-safe manner
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

				//distances in coordinates
				float diff_x=left_loc.x-right_loc.x;
				float diff_y=left_loc.y-right_loc.y;
				
				float distance = sqrt((diff_x)*(diff_x)+(diff_y)*(diff_y));

				//compute the actual gravity force vector
				sf::Vector2f gravity_force_vec={diff_x,diff_y};
				if(distance!=0) 
					gravity_force_vec*=(G*left_mass*right_mass)/(distance*distance*distance); //proper force of gravity
				else
					gravity_force_vec= {0,0};
				
				//update targets in a thread-safe manner
				//velocities are being altered accordingly to the force of gravity and stepping rate
				lhs->simultaneity_guardian.lock();
				lhs->set_velocity(left_v-gravity_force_vec/(float)left_mass*STEPPING_RATE);
				lhs->simultaneity_guardian.unlock();

				rhs->simultaneity_guardian.lock();
				rhs->set_velocity(right_v+gravity_force_vec/(float)right_mass*STEPPING_RATE);
				rhs->simultaneity_guardian.unlock();
				
			};
			
			simultaneous_computator.async_pairwise_apply([compute_gravity](const std::list<std::unique_ptr<Celestial_body>>::iterator& one,const std::list<std::unique_ptr<Celestial_body>>::iterator& two) {
				compute_gravity(one->get(),two->get());
			});

			//checks for collisions between pairs of celestial bodies and stores results in a vector
			//THREAD-SAFE
			auto detect_collisions = [](std::vector<std::pair<unsigned int,unsigned int>>& detected, std::mutex& detected_mutex,const std::list<std::unique_ptr<Celestial_body>>::iterator lhs,const std::list<std::unique_ptr<Celestial_body>>::iterator rhs)
			{
				auto* lhs_ptr = lhs->get();
				auto* rhs_ptr = rhs->get();
				if(lhs_ptr == nullptr || rhs_ptr == nullptr)
					throw std::runtime_error("Tried to compute gravity with a null-pointer.");
				
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
			
			//pass a bind-expression to simultaneous computator to proper handle references.
			simultaneous_computator.async_pairwise_apply(std::bind(detect_collisions,std::ref(detected_pairs),std::ref(detected_mutex),std::placeholders::_1,std::placeholders::_2));
			
			for(auto& x: detected_pairs) 
			{
				//if neither of the bodies in the current pair of collided bodies were handled before
				if(deleted_bodies.find(x.first) == deleted_bodies.end() && deleted_bodies.find(x.second)==deleted_bodies.end())
				{
					auto father_iterator = iterator_of(x.first);
					auto mother_iterator = iterator_of(x.second);
					switch(ca)
					{
						case collision_approach::merge:
						{
							//the second, being a father, always gets overwritten by the child. first (mother) is to be deleted manually
							Celestial_body* father = father_iterator->release();
							deleted_bodies.insert(x.first); //mark the father as a deleted body because it will be overwritten
							deleted_bodies.insert(x.second);//this one gets deleted too
							Celestial_body::collision_handle(mother_iterator->get(),father); //father is being overwritten (becomes child)
							father_iterator->reset(father); //father is now the child
							
							body_list.erase(mother_iterator); 
							break;
						}
						case collision_approach::bounce:
						{
							Celestial_body::bounce_handle(mother_iterator->get(),father_iterator->get());
							break;
						}
						case collision_approach::mixed:
						{
							// if(distance < sum of radii - overlap_tolerance * smallest radius)
							if(mother_iterator->get()->distance_from(*father_iterator->get()) < father_iterator->get()->get_radius() + mother_iterator->get()->get_radius() - overlap_tolerance * std::min(father_iterator->get()->get_radius(),mother_iterator->get()->get_radius())) 
							{
								//the second, being a father, always gets overwritten by the child. the first (mother) is to be deleted manually
								Celestial_body* father = father_iterator->release();
								deleted_bodies.insert(x.first); //mark the father as a deleted body because it will be overwritten
								deleted_bodies.insert(x.second);//this one gets deleted too
								Celestial_body::collision_handle(mother_iterator->get(),father); //father is being overwritten (becomes child)
								father_iterator->reset(father); //father is now the child

								body_list.erase(mother_iterator); 
							}
							else
							{
								Celestial_body::bounce_handle(mother_iterator->get(),father_iterator->get());
							}
							break;
						}
					}
				}
			}

			//set locations according to velocities and stepping rate
			for(auto j=body_list.begin(); j!=body_list.end(); j++)
			{
				auto q=j->get();
				q->set_location(q->get_location()+(q->get_velocity())*STEPPING_RATE); 
			}

		}

	}
}


void Simulator::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	for(auto& x: body_list)
	{
		if(!paused) 
			x->refresh(); //refresh the state of the graphical part of the object
		if(draw_traces) 
			x->draw_trace(tgt,st);
		
	}
	for(auto& x: body_list) //has to be drawn in a separate loop no to overlap with the traces
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
	body_list.push_back(std::unique_ptr<Celestial_body>(koles));
}

void Simulator::erase_body(unsigned int ajdi)
{
	for(auto i=body_list.begin(); i!=body_list.end(); i++) 
		if(i->get()->get_id()==ajdi) 
		{
			erase_body(i); 
			return;
		}
}

std::list<std::unique_ptr<Celestial_body>>::iterator Simulator::erase_body(const std::list<std::unique_ptr<Celestial_body>>::iterator& el)
{
	return body_list.erase(el);
}

std::list<std::unique_ptr<Celestial_body>>::const_iterator Simulator::get_end() const
{
	return body_list.cend();
}

std::list<std::unique_ptr<Celestial_body>>::iterator Simulator::iterator_of(unsigned int ajdi)
{
	for(auto i=body_list.begin();i!=body_list.end();i++)
	{
		if(i->get()->get_id()==ajdi) return i;
	}
	throw std::runtime_error("Requested iterator of non-existent object with ID: "+std::to_string(ajdi));
}

std::list<std::unique_ptr<Celestial_body>>::iterator Simulator::at_pos(const sf::Vector2f& here)
{
	
	for(auto i=body_list.begin(); i!=body_list.end(); i++) //i jest iteratorem listy
	{
		auto w=i->get()->getGlobalBounds();

		if(w.contains(here))
			return i;
		
		
		
	}

	return body_list.end();
	
}

void Simulator::pause(bool rzejak)
{
	paused=rzejak;
}

bool Simulator::pause() const
{
	return paused;
}

void Simulator::toggle_traces()
{
	draw_traces=!draw_traces;
}

void Simulator::delete_traces()
{
	for(auto& x: body_list) x->delete_traces();
}

bool Simulator::are_traces_drawn() const
{
	return draw_traces;
}

std::list<std::vector<sf::Vertex>> Simulator::get_traces()
{
	std::list<std::vector<sf::Vertex>> ret;
	for(auto& x: body_list) ret.splice(ret.begin(),x->get_traces());
	return ret;
}

void Simulator::predict_traces()
{
	//to be implemented
	Simulator future_teller(*this);
}

Simulator::collision_approach Simulator::cycle_collision_approach()
{
	unsigned short temp_ca = (unsigned short)ca; //ca stands for collision approach strategy (enum)
	temp_ca++;
	if(temp_ca>CA_count) 
		temp_ca = 1;
	ca = (collision_approach)temp_ca;
	return ca;
}

Simulator::Simulator()  
:body_list(),
simultaneous_computator(body_list)
{
	Celestial_body::pushstax();
	paused = false;
	draw_traces = true;
	ca = collision_approach::merge;
	predicted_traces= nullptr;
}

Simulator::Simulator(const Simulator &sim) 
:body_list(),
simultaneous_computator(body_list)
{
	Celestial_body::pushstax();
	paused = sim.paused;
	draw_traces = sim.draw_traces;
	ca = sim.ca;
	predicted_traces = nullptr;
	
	
	
	for(auto i=sim.body_list.begin(); i!=sim.body_list.end(); i++)
	{
		Celestial_body* buffer = nullptr;
		
		Celestial_body* old_body_ptr = i->get(); 
		
		buffer = old_body_ptr->clone(*old_body_ptr); //use RTTI to clone objects with valid type
		
		body_list.push_back(std::unique_ptr<Celestial_body>(buffer));

	}
	
	
	
}

Simulator::~Simulator()
{
	Celestial_body::popstax();
}


