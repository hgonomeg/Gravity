#include "Celestial_body.hpp"
#include "Space_objects.hpp"

unsigned int Celestial_body::Global_ID=0;
unsigned int Celestial_body::znikacz_sladu=5;
unsigned int Celestial_body::num_of_polygon_sides=64;
std::map<unsigned int, unsigned int> Celestial_body::alloc_diagram = std::map<unsigned int, unsigned int>();
std::stack<std::pair<std::map<unsigned int, unsigned int>,unsigned int>> Celestial_body::alloc_diagram_stack = std::stack<std::pair<std::map<unsigned int, unsigned int>,unsigned int>>();

void Celestial_body::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(wyglond,st);
}

void Celestial_body::draw_trace(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(&slad[0],slad.size(),sf::LineStrip,st);

	if(slady_rodzicow)
	{
		for(auto& x: (*slady_rodzicow)) tgt.draw(&x[0],x.size(),sf::LineStrip,st);
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
	return loc;
}
sf::Vector2f Celestial_body::get_velocity() const
{
	return v;
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
	loc = lc;
}

void Celestial_body::set_velocity(sf::Vector2f vel) 
{
	v = vel;
}

void Celestial_body::refresh()
{

		if(rc>=znikacz_sladu) rc=0;
		if(rc==0)
		{
			for(auto iter=slad.begin();iter!=slad.end();iter++)
			{
				
				if(iter->color.a==0) purge++;
				else
				{
					iter->color.a--;
				}
				if(purge>125000)
				{
					purge=0;
					slad.erase(slad.begin(),std::find_if(slad.rbegin(),slad.rend(),[](const sf::Vertex& xe){return xe.color.a==0;}).base()); //slad jest wektorem którego usuwamy początek
					break;	
				}
				
					
			}
			if(slady_rodzicow)
			{
				for(auto u=slady_rodzicow->begin();u!=slady_rodzicow->end();u++)
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
						u=slady_rodzicow->erase(u); 
						if(u==slady_rodzicow->end()) break;
						}
				}
				if(slady_rodzicow->size()==0) {delete slady_rodzicow; slady_rodzicow = nullptr; }
			}
		}
		rc++;
		if(!is_still&&v.x!=0.f&&v.y!=0.f) slad.emplace_back(sf::Vertex(wyglond.getPosition(),tracecolor));
		if(is_still) {loc=wyglond.getPosition(); v={0,0};}
		wyglond.setPosition(loc);
}


unsigned int Celestial_body::get_id() const
{
	std::lock_guard<std::mutex> mlock(simultaneity_guardian);
	return Local_ID;
}

Celestial_body::Celestial_body(int imass,const sf::Color& kolorek,const sf::Vector2f& iloc,const sf::Vector2f& iv)
{
	mass = imass;
	loc = iloc;
	v = iv;
	rc = 0;
	tracecolor = sf::Color(255,255,255);
	purge = 0;
	is_still = false;
	radius = sqrt(fabs(mass/10.f));
	wyglond.setRadius(radius);
	wyglond.setPointCount(num_of_polygon_sides);
	wyglond.setOrigin(radius,radius);
	wyglond.setOutlineThickness(0);
	wyglond.setFillColor(kolorek);
	if(mass<0) wyglond.setFillColor(sf::Color(255,200,255));
	wyglond.setPosition(loc);
	slad.emplace_back(sf::Vertex(loc,tracecolor));
	Local_ID=Global_ID; 
	alloc_diagram.emplace(Local_ID,Local_ID);
	Global_ID++;
	slady_rodzicow = nullptr;
}

Celestial_body::Celestial_body(const Celestial_body& rhs) // konstuktor kopujacy
{
	mass = rhs.mass;
	loc = rhs.loc;
	v = rhs.v;
	rc = rhs.rc;
	tracecolor = rhs.tracecolor;
	purge = rhs.purge;
	radius = rhs.radius;
	wyglond = rhs.wyglond;
	tex = rhs.tex;
	slad = rhs.slad;
	is_still = rhs.is_still;
	Local_ID=Global_ID; 
	alloc_diagram.emplace(Local_ID,Local_ID);
	if(rhs.slady_rodzicow)
	{
		slady_rodzicow = new std::list<std::vector<sf::Vertex>>(*rhs.slady_rodzicow);
	}
	else slady_rodzicow=nullptr;
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
	wyglond.setPointCount(sides);
}

bool Celestial_body::change_trace_length(bool dir)
{
	if(dir)
	{
		znikacz_sladu++;
		return true;
	}
	else
	{
		if(znikacz_sladu==0) return false;
		znikacz_sladu--;
		return true;
	}
}

unsigned int Celestial_body::get_trace_length()
{
	return znikacz_sladu;
}

const std::map<unsigned int, unsigned int>& Celestial_body::get_alloc_diagram()
{
	return alloc_diagram;
}

Celestial_body::~Celestial_body()
{
	if(slady_rodzicow) delete slady_rodzicow;
	
}

std::list<std::vector<sf::Vertex>> Celestial_body::get_traces()
{
	std::list<std::vector<sf::Vertex>> ret;
	ret.push_back(slad);
	if(slady_rodzicow)
	{
		for(auto i=slady_rodzicow->begin();i!=slady_rodzicow->end();i++) ret.push_back(*i);
	}
	return ret;
}



void Celestial_body::collision_handle(Celestial_body* matka, Celestial_body*& ojciec)
{
	auto&& S_m=matka->get_traces();
	auto&& S_o=ojciec->get_traces();
	
	sf::Vector2f V_d;
	int M_d;
	
	//gettery
	
	int M_m=matka->get_mass();
	int M_o=ojciec->get_mass();
	
	auto V_m=matka->get_velocity();
	auto V_o=ojciec->get_velocity();
	
	//obliczenia
	
	M_d=M_m+M_o;
	
	Planet* planeta;
	Star* gwiazda;
	Asteroid* asteroida;

	Celestial_body* dziecko;
	
	if(M_d==0)
	{
		V_d=(((float)M_m*V_m)+((float)M_o*V_o))/(M_m*2.f);
		dziecko=new Asteroid(matka->get_location(),V_d);
	}
	else
	{
		V_d=(((float)M_m*V_m)+((float)M_o*V_o))/(float)M_d;

		if(M_m>=M_o) //tu dziecko przyjmnie klase matki // sprawdzamy jaką matka i ojciec są klasą
		{
			planeta=dynamic_cast<Planet*>(matka);
			gwiazda=dynamic_cast<Star*>(matka);
			asteroida=dynamic_cast<Asteroid*>(matka);
			if(planeta==nullptr&&asteroida==nullptr) //obiekt będzie gwiazdą
			{
				dziecko=new Star(M_d,matka->get_location(),V_d);
				dziecko->is_still=matka->is_still;
			}
			else if(asteroida!=nullptr)
			{
				dziecko=new Asteroid(matka->get_location(),V_d);
			}
			else //obiekt będzie planetą
			{
				dziecko=new Planet(M_d,matka->get_location(),V_d);
				dziecko->is_still=matka->is_still;
			}
		}	
		else //tu dziecko przyjmie klase ojca
		{
			planeta=dynamic_cast<Planet*>(ojciec);
			gwiazda=dynamic_cast<Star*>(ojciec);
			asteroida=dynamic_cast<Asteroid*>(ojciec);
			if(planeta==nullptr&&asteroida==nullptr) // gwiazda
			{
				dziecko=new Star(M_d,ojciec->get_location(),V_d);
				dziecko->is_still=ojciec->is_still;
			}
			else if(asteroida!=nullptr)
			{
				dziecko=new Asteroid(ojciec->get_location(),V_d);
			}
			else //obiekt będzie planetą
			{
				dziecko=new Planet(M_d,ojciec->get_location(),V_d);
				dziecko->is_still=ojciec->is_still;
			}	
		}	

	}
	dziecko->slady_rodzicow = new std::list<std::vector<sf::Vertex>>();
	for(auto i=S_o.begin();i!=S_o.end();i++)
	{
		dziecko->slady_rodzicow->push_back(*i);
	}
	for(auto i=S_m.begin();i!=S_m.end();i++)
	{
		dziecko->slady_rodzicow->push_back(*i);
	}
	
	alloc_diagram[ojciec->get_id()]=dziecko->get_id();
	alloc_diagram[matka->get_id()]=dziecko->get_id();
	
	
	delete ojciec; ojciec = dziecko;
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
		
		float odleglosc= sqrt((diff_x)*(diff_x)+(diff_y)*(diff_y));
		
		return odleglosc;
	
}

void Celestial_body::delete_traces()
{
	slad.clear();
	slad.shrink_to_fit();
	if(slady_rodzicow) delete slady_rodzicow;
	slady_rodzicow=nullptr;
}

void Celestial_body::bounce_handle(Celestial_body* matka, Celestial_body* ojciec)
{
	 // 1. W momencie zderzenia wyznaczamy współrzędne środków zderzających się obiektów i numerujemy te obiekty.
 
    int M_m=matka->get_mass();
	int M_o=ojciec->get_mass();
	
	sf::Vector2f V_m=matka->get_velocity();
	sf::Vector2f V_o=ojciec->get_velocity();
	
	sf::Vector2f loc_m=matka->get_location();
	sf::Vector2f loc_o=ojciec->get_location();
	
    // 2. Wyznaczamy wektor r12 różnicy położeń tych ciał.
 
    sf::Vector2f S1S2 = loc_m-loc_o;
    sf::Vector2f S2S1 = loc_o-loc_m;
 
    auto vec_dot_product = [](sf::Vector2f pierwszy, sf::Vector2f drugi) -> float {return (pierwszy.x*drugi.x)+(pierwszy.y*drugi.y);}; //iloczyn skalarny
 
    auto powered_vec_length = [](sf::Vector2f vec) -> float {return vec.x*vec.x+vec.y*vec.y;}; //długość wektora
	
	// 3. Rzutujemy wektor V_m oraz wektor V_o na kierunek wektora r12 (można to zrobić bez użycia równania prostej, można wykorzystać własność iloczynu skalarnego, w ten sposób otrzymamy konkretne współrzędne wektora vC1 oraz vc2 (składowych prędkości wzdłuż prostej łączącej środki kul)).

    auto vec_projection = [&](sf::Vector2f pierwszy, sf::Vector2f drugi) -> sf::Vector2f {return static_cast<float>(vec_dot_product(pierwszy,drugi)/powered_vec_length(drugi))*drugi;}; //pierwszy rzucamy na drugi
 
 
    //rzut V_m i V_o
    sf::Vector2f V_mc = vec_projection(V_m,S1S2);
    sf::Vector2f V_oc = vec_projection(V_o,S2S1);
 
    //4. Szukamy prostopadłych do tej prostej składowych wektorów V_m i V_o, np. poprzez różnicę wektorową: vp1 = V_m - vC1 ; vp2 = V_o-vC2.
 
    sf::Vector2f V_mp = V_m-V_mc;
    sf::Vector2f V_op = V_o-V_oc;
 
    //5. Zderzenie sprężyste zachodzi tak, że składowe vC1 i vC2 przetransformują się wg wzorów opisujących zderzenie centralne (które znaleźliście), natomiast składowe vp1 i vp2 pozostaną bez zmian (gdyby kule miały jedynie składowe vp1 i vp2 (tzn V_m = vp1 oraz V_o = vp2, to kule jedynie "musnęłyby się" bez zmiany prędkości)).
    //transformacja V_mc i V_oc na U1c i U2c
 
    sf::Vector2f U1c = static_cast<float>((M_m-M_o)/(M_m+M_o))*V_mc + static_cast<float>((2*M_o)/(M_m+M_o))*V_oc; //z wzorów na zderzenie centralne sprężyste
    sf::Vector2f U2c = static_cast<float>((2*M_m)/(M_m+M_o))*V_mc + static_cast<float>((M_o-M_m)/(M_m+M_o))*V_oc;
 
    //6. Wektor prędkości po zderzeniu jest równy u1 = vp1 + uC1  ;  u2 = vp2 + uC2 ; gdzie uC1 i uC2 to przetransformowane wektory vC1 i vC2 wg wzorów opisujących zderzenie sprężyste centralne.
 
    V_m = U1c + V_mp;
	V_o = U2c + V_op;

	matka->set_velocity(V_m);
	ojciec->set_velocity(V_o);

}

sf::FloatRect Celestial_body::getGlobalBounds()
{
	return wyglond.getGlobalBounds();
}


bool Celestial_body::collision_detection(const Celestial_body& CB1, const Celestial_body& CB2)
{
	CB1.simultaneity_guardian.lock();
	auto radius1 = CB1.get_radius();
	CB1.simultaneity_guardian.unlock();
	CB2.simultaneity_guardian.lock();
	auto radius2 = CB2.get_radius();
	CB2.simultaneity_guardian.unlock();
	return (CB1.distance_from(CB2)-(radius1+radius2))<=static_cast<float>(0);
}
