#include "Celestial_body.hpp"
#include "Space_objects.hpp"

unsigned int Celestial_body::Global_ID=0;
unsigned int Celestial_body::trace_fading_cycle_interval=5;
unsigned int Celestial_body::num_of_polygon_sides=64;
std::map<unsigned int, unsigned int> Celestial_body::alloc_diagram = std::map<unsigned int, unsigned int>();
std::stack<std::pair<std::map<unsigned int, unsigned int>,unsigned int>> Celestial_body::alloc_diagram_stack = std::stack<std::pair<std::map<unsigned int, unsigned int>,unsigned int>>();

void Celestial_body::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(body_sprite,st);
}

void Celestial_body::draw_trace(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(&trace[0],trace.size(),sf::LineStrip,st);

	if(parents_traces)
	{
		for(auto& x: (*parents_traces)) tgt.draw(&x[0],x.size(),sf::LineStrip,st);
	}
}

int Celestial_body::get_mass() const
{
	return mass;
}
float Celestial_body::get_radius() const
{
	return radius;
}
sf::Vector2f Celestial_body::get_location() const
{
	return location;
}
sf::Vector2f Celestial_body::get_velocity() const
{
	return velocity;
}

void Celestial_body::set_mass(int m)
{
	mass = m;
}

void Celestial_body::set_radius(float rad)
{
	radius = rad;
}

void Celestial_body::set_location(sf::Vector2f lc)
{
	location = lc;
}

void Celestial_body::set_velocity(sf::Vector2f vel) 
{
	velocity = vel;
}

void Celestial_body::refresh()
{

		if(trace_fading_refresh_cycles>=trace_fading_cycle_interval) 
			trace_fading_refresh_cycles=0;
		//this gets executed every <trace_fading_cycle_interval> calls to refresh()
		//which provides a requlation mechanism for the pace of fading of celestial body traces
		if(trace_fading_refresh_cycles==0)
		{
			for(auto iter=trace.begin();iter!=trace.end();iter++)
			{
				
				if(iter->color.a==0) trace_trim_counter++; //notify that transparent nodes build up
				else
				{
					iter->color.a--; //fade the current node
				}
				if(trace_trim_counter>125000) //an arbitrary fixed value
				{
					trace_trim_counter=0;
					//the following statement trims the trace vector:
					//transparent (faded) trace nodes pile up at the beginning of the vector (since they're the oldest)
					//std::find_if goes from the back of the vector (.rbegin()) and returns the first element satisfying the condition given by lambda (the first transparent node, counting from the back)
					//therefore the vector's beginning gets deleted up to the position of the last faded node (counting from beginning)
					trace.erase(trace.begin(),std::find_if(trace.rbegin(),trace.rend(),[](const sf::Vertex& vtx)
																						{
																							return vtx.color.a==0; //true for transparent
																						}).base()); //since std::find_if is used with reverse iterators, we need to call base() to get a normal iterator
					break;	
				}
				
					
			}
			if(parents_traces)
			{
				for(auto u=parents_traces->begin();u!=parents_traces->end();u++)
				{
					for(auto i=u->begin();i!=u->end();i++)
					{
						if(i->color.a>0)
						{
							i->color.a--; //fade each trace node gradually to 0
						}
					}
					if(u->back().color.a==0) //when the last node of a trace gets transparent, it means that this particular trace can be disposed of entirely because it is all transparent
						{
						u=parents_traces->erase(u); 
						if(u==parents_traces->end()) break;
						}
				}
				if(parents_traces->size()==0) //when there is no parent traces left, delete the whole vector
				{
					delete parents_traces; 
					parents_traces = nullptr; 
				}
			}
		}
		trace_fading_refresh_cycles++;
		if(!is_still && velocity.x!=0.f && velocity.y!=0.f) //when it is reasonable to produce a trace node, do it
			trace.emplace_back(sf::Vertex(body_sprite.getPosition(),trace_color));
		if(is_still) //when the celestial body is supposed to occupy a fixed place
		{
			location=body_sprite.getPosition(); //freeze the numeric value of location to remain where the sprite is
			velocity={0,0}; 
		}
		body_sprite.setPosition(location); //move the sprite to reflect a possible change in body's position
}


unsigned int Celestial_body::get_id() const
{
	std::lock_guard<std::mutex> mlock(simultaneity_guardian);
	return Local_ID;
}

Celestial_body::Celestial_body(int imass,const sf::Color& icolor,const sf::Vector2f& iloc,const sf::Vector2f& iv)
{
	mass = imass;
	location = iloc;
	velocity = iv;
	trace_fading_refresh_cycles = 0;
	trace_color = sf::Color(255,255,255);
	trace_trim_counter = 0;
	is_still = false;
	radius = sqrt(fabs(mass/10.f));
	body_sprite.setRadius(radius);
	body_sprite.setPointCount(num_of_polygon_sides);
	body_sprite.setOrigin(radius,radius);
	body_sprite.setOutlineThickness(0);
	body_sprite.setFillColor(icolor);
	if(mass<0) //special color for experimental negative mass
		body_sprite.setFillColor(sf::Color(255,200,255));
	body_sprite.setPosition(location);
	trace.emplace_back(sf::Vertex(location,trace_color));
	Local_ID=Global_ID; 
	alloc_diagram.emplace(Local_ID,Local_ID);
	Global_ID++;
	parents_traces = nullptr;
}

Celestial_body::Celestial_body(const Celestial_body& rhs)
{
	mass = rhs.mass;
	location = rhs.location;
	velocity = rhs.velocity;
	trace_fading_refresh_cycles = rhs.trace_fading_refresh_cycles;
	trace_color = rhs.trace_color;
	trace_trim_counter = rhs.trace_trim_counter;
	radius = rhs.radius;
	body_sprite = rhs.body_sprite;
	trace = rhs.trace;
	is_still = rhs.is_still;
	Local_ID=Global_ID; 
	alloc_diagram.emplace(Local_ID,Local_ID);
	if(rhs.parents_traces)
	{
		parents_traces = new std::list<std::vector<sf::Vertex>>(*rhs.parents_traces);
	}
	else parents_traces=nullptr;
}

void Celestial_body::popstax()
{
	if(alloc_diagram_stack.size()>0)
	{
		std::pair<std::map<unsigned int, unsigned int>,unsigned int> pya = alloc_diagram_stack.top();
		alloc_diagram_stack.pop();
		Global_ID=pya.second;
		alloc_diagram=pya.first;
	}
}

void Celestial_body::pushstax()
{
	std::pair<std::map<unsigned int, unsigned int>,unsigned int> pya;
	pya.second=Global_ID;
	pya.first=alloc_diagram;
	alloc_diagram_stack.push(pya);
}

void Celestial_body::set_global_num_of_polygon_sides(unsigned int sides)
{
	num_of_polygon_sides = sides;
}
void Celestial_body::set_num_of_polygon_sides(unsigned int sides)
{
	body_sprite.setPointCount(sides);
}

bool Celestial_body::change_trace_length(bool dir)
{
	if(dir)
	{
		trace_fading_cycle_interval++;
		return true;
	}
	else
	{
		if(trace_fading_cycle_interval==0) return false;
		trace_fading_cycle_interval--;
		return true;
	}
}

unsigned int Celestial_body::get_trace_length()
{
	return trace_fading_cycle_interval;
}

const std::map<unsigned int, unsigned int>& Celestial_body::get_alloc_diagram()
{
	return alloc_diagram;
}

Celestial_body::~Celestial_body()
{
	if(parents_traces) 
		delete parents_traces;
}

std::list<std::vector<sf::Vertex>> Celestial_body::get_traces()
{
	std::list<std::vector<sf::Vertex>> ret;
	ret.push_back(trace);
	if(parents_traces)
	{
		for(auto i=parents_traces->begin();i!=parents_traces->end();i++) ret.push_back(*i);
	}
	return ret;
}



void Celestial_body::collision_handle(Celestial_body* mother, Celestial_body*& father)
{
	sf::Vector2f child_velocity;
	int child_mass;

	//fetch data

	auto&& mother_traces = mother->get_traces();
	auto&& father_traces = father->get_traces();
	
	int mother_mass = mother->get_mass();
	int father_mass = father->get_mass();
	
	auto mother_velocity = mother->get_velocity();
	auto father_velocity = father->get_velocity();
	
	//calculations
	
	child_mass = mother_mass+father_mass;

	Celestial_body* child;
	
	if(child_mass==0)
	{
		child_velocity = (((float)mother_mass*mother_velocity)+((float)father_mass*father_velocity))/(mother_mass*2.f);
		child=new Asteroid(mother->get_location(),child_velocity);
	}
	else
	{
		child_velocity = (((float)mother_mass*mother_velocity)+((float)father_mass*father_velocity))/(float)child_mass;

		Celestial_body* master_parent; //master parent is the literally heavier parent. It will decide about child's body type (RTTI dynamic_casts)
		
		//pointers for RTTI checks

		Planet* planet;
		Star* star;
		Asteroid* asteroid;

		if(mother_mass>=father_mass)
			master_parent = mother;
		else
			master_parent = father;
		
		planet = dynamic_cast<Planet*>(master_parent);
		star = dynamic_cast<Star*>(master_parent);
		asteroid = dynamic_cast<Asteroid*>(master_parent);

		if(planet==nullptr && asteroid==nullptr) //child ought to be a star
			child = new Star(child_mass,master_parent->get_location(),child_velocity,master_parent->is_still);

		else if(asteroid!=nullptr) //child ought to be an asteroid
			child = new Asteroid(master_parent->get_location(),child_velocity);

		else //child ought to be a planet
			child = new Planet(child_mass,master_parent->get_location(),child_velocity);
			
		child->body_sprite.setTexture(master_parent->body_sprite.getTexture());

	}

	child->parents_traces = new std::list<std::vector<sf::Vertex>>();

	for(auto i=father_traces.begin();i!=father_traces.end();i++)
		child->parents_traces->push_back(*i);

	for(auto i=mother_traces.begin();i!=mother_traces.end();i++)
		child->parents_traces->push_back(*i);
	
	alloc_diagram[father->get_id()] = child->get_id();
	alloc_diagram[mother->get_id()] = child->get_id();
	
	
	delete father; 
	father = child; //father is technically a reference to a pointer and therefore gets overwritten by child
	//mother is to be deleted manually after this function is called
}

float Celestial_body::distance_from(const Celestial_body& ext) const
{
		this->simultaneity_guardian.lock();
		auto left_loc=this->get_location();
		this->simultaneity_guardian.unlock();

		ext.simultaneity_guardian.lock();
		auto right_loc=ext.get_location();
		ext.simultaneity_guardian.unlock();

		float diff_x=left_loc.x-right_loc.x;
		float diff_y=left_loc.y-right_loc.y;
		
		float distance = sqrt((diff_x)*(diff_x)+(diff_y)*(diff_y));
		
		return distance;
	
}

void Celestial_body::delete_traces()
{
	trace.clear();
	trace.shrink_to_fit();
	if(parents_traces) 
		delete parents_traces;
	parents_traces = nullptr;
}

//HELPER FUNCTIONS FOR Celestial_body::bounce_handle(Celestial_body*,Celestial_body*)
//this does scalar vector multiplication
inline float vec_dot_product(const sf::Vector2f& first, const sf::Vector2f& second)
{
	return (first.x*second.x)+(first.y*second.y);
}; 

inline float squared_vec_length(const sf::Vector2f& vec)
{
	return vec.x*vec.x+vec.y*vec.y;
};

//vector projection: project first onto second
sf::Vector2f vec_projection(const sf::Vector2f& first,const sf::Vector2f& second)
{
	return static_cast<float>(vec_dot_product(first,second)/squared_vec_length(second))*second;
}; 

//uses helper functions defined above
void Celestial_body::bounce_handle(Celestial_body* mother, Celestial_body* father)
{
	 // 1. Fetch parameters of colliding objects
 
    int mother_mass=mother->get_mass();
	int father_mass=father->get_mass();
	
	sf::Vector2f mother_velocity=mother->get_velocity();
	sf::Vector2f father_velocity=father->get_velocity();
	
	sf::Vector2f mother_location=mother->get_location();
	sf::Vector2f father_location=father->get_location();
	
    // 2. Compute vectors of difference of coordinates
 
    sf::Vector2f diff_father_to_mother = mother_location-father_location;
    sf::Vector2f diff_mother_to_father = father_location-mother_location;

	// 3. Project mother and father velocity vectors onto the diff vectors created above 

    //This vector projection computes the velocity along our axis of collision
    sf::Vector2f mother_velocity_axial = vec_projection(mother_velocity,diff_father_to_mother);
    sf::Vector2f father_velocity_axial = vec_projection(father_velocity,diff_mother_to_father);
 
    //4. Compute vectors reflecting velocities perpendicular to the collision axis
 
    sf::Vector2f mother_velocity_perpendic = mother_velocity-mother_velocity_axial;
    sf::Vector2f father_velocity_perpendic = father_velocity-father_velocity_axial;
 
	//5. For elastic collisions, parts of velocities aligned along the collision axis behave like a regular central elastic collision
	
	//from physical formulas:
    sf::Vector2f mother_velocity_collision = static_cast<float>((mother_mass-father_mass)/(mother_mass+father_mass))*mother_velocity_axial + static_cast<float>((2*father_mass)/(mother_mass+father_mass))*father_velocity_axial; 
    sf::Vector2f father_velocity_collision = static_cast<float>((2*mother_mass)/(mother_mass+father_mass))*mother_velocity_axial + static_cast<float>((father_mass-mother_mass)/(mother_mass+father_mass))*father_velocity_axial;
 
    //6. Compute the final vectors after collision:
 
    mother_velocity = mother_velocity_collision + mother_velocity_perpendic;
	father_velocity = father_velocity_collision + father_velocity_perpendic;

	mother->set_velocity(mother_velocity);
	father->set_velocity(father_velocity);

}

sf::FloatRect Celestial_body::getGlobalBounds()
{
	return body_sprite.getGlobalBounds();
}


bool Celestial_body::collision_detection(const Celestial_body& lhs, const Celestial_body& rhs)
{
	lhs.simultaneity_guardian.lock();
	auto radius1 = lhs.get_radius();
	lhs.simultaneity_guardian.unlock();
	rhs.simultaneity_guardian.lock();
	auto radius2 = rhs.get_radius();
	rhs.simultaneity_guardian.unlock();
	return (lhs.distance_from(rhs)-(radius1+radius2))<=static_cast<float>(0);
}

unsigned short Celestial_body::random_number(unsigned short cap)
{
	static std::default_random_engine generator;
	std::uniform_int_distribution<unsigned short> distribution(0,cap);
	return distribution(generator);
}
