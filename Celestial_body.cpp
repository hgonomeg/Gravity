#include "Celestial_body.hpp"
#include "Space_objects.hpp"

unsigned int Celestial_body::Global_ID=0;
unsigned int Celestial_body::znikacz_sladu=5;
std::map<unsigned int, unsigned int> Celestial_body::alloc_diagram = std::map<unsigned int, unsigned int>();

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

int& Celestial_body::get_mass() 
{
	return mass;
}
float& Celestial_body::get_radius() 
{
	return radius;
}
sf::Vector2f& Celestial_body::get_loc()
{
	return loc;
}
sf::Vector2f& Celestial_body::get_v()
{
	return v;
}

void Celestial_body::refresh()
{

		if(rc>=znikacz_sladu) rc=0;
		if(rc==0)
		{
			for(auto iter=slad.begin();iter!=slad.end();iter++)
			{
				//if(iter==slad.begin()) std::cout<<(int)iter->color.r<<'\n';
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
				if(slady_rodzicow->size()==0) {delete slady_rodzicow; slady_rodzicow = NULL; }
			}
		}
		rc++;
		slad.emplace_back(sf::Vertex(wyglond.getPosition(),tracecolor));
		wyglond.setPosition(loc);
}


unsigned int Celestial_body::get_id()
{
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
	radius = sqrt(mass/10.f);
	wyglond.setRadius(radius);
	wyglond.setPointCount(64);
	wyglond.setOrigin(radius,radius);
	wyglond.setOutlineThickness(0);
	wyglond.setFillColor(kolorek);
	wyglond.setPosition(loc);
	slad.emplace_back(sf::Vertex(loc,tracecolor));
	Local_ID=Global_ID; 
	alloc_diagram.emplace(Local_ID,Local_ID);
	Global_ID++;
	slady_rodzicow = NULL;
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
	Local_ID=Global_ID; 
	alloc_diagram.emplace(Local_ID,Local_ID);
	if(rhs.slady_rodzicow)
	{
		slady_rodzicow = new std::list<std::vector<sf::Vertex>>(*rhs.slady_rodzicow);
	}
	else slady_rodzicow=NULL;
}

const std::map<unsigned int, unsigned int>& Celestial_body::get_alloc_diagram()
{
	return alloc_diagram;
}

Celestial_body::~Celestial_body()
{
	//alloc_diagram[Local_ID]=false;
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
	
	int& M_m=matka->get_mass();
	int& M_o=ojciec->get_mass();
	
	auto& V_m=matka->get_v();
	auto& V_o=ojciec->get_v();
	
	//obliczenia
	
	M_d=M_m+M_o;
	V_d=(((float)M_m*V_m)+((float)M_o*V_o))/(float)M_d;
	
	Planet* planeta;
	Star* gwiazda;
	Celestial_body* dziecko;
	Asteroid* asteroida;
	
	if(M_m>=M_o) //tu dziecko przyjmnie klase matki // sprawdzamy jaką matka i ojciec są klasą
	{
		planeta=dynamic_cast<Planet*>(matka);
		gwiazda=dynamic_cast<Star*>(matka);
		asteroida=dynamic_cast<Asteroid*>(matka);
		if(planeta==NULL&&asteroida==NULL) //obiekt będzie gwiazdą
		{
			dziecko=new Star(M_d,matka->get_loc(),V_d);
		}
		else if(asteroida!=NULL)
		{
			dziecko=new Asteroid(matka->get_loc(),V_d);
		}
		else //obiekt będzie planetą
		{
			dziecko=new Planet(M_d,matka->get_loc(),V_d);
		}
	}	
	else //tu dziecko przyjmie klase ojca
	{
		planeta=dynamic_cast<Planet*>(ojciec);
		gwiazda=dynamic_cast<Star*>(ojciec);
		asteroida=dynamic_cast<Asteroid*>(ojciec);
		if(planeta==NULL&&asteroida==NULL) // gwiazda
		{
			dziecko=new Star(M_d,ojciec->get_loc(),V_d);
		}
		else if(asteroida!=NULL)
		{
			dziecko=new Asteroid(ojciec->get_loc(),V_d);
		}
		else //obiekt będzie planetą
		{
			dziecko=new Planet(M_d,ojciec->get_loc(),V_d);
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

float Celestial_body::distance_from(Celestial_body* CB1, Celestial_body* CB2)
{
		auto &left_loc=CB1->get_loc();

		auto &right_loc=CB2->get_loc();

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
	slady_rodzicow=NULL;
}

void Celestial_body::bounce_handle(Celestial_body* matka, Celestial_body* ojciec)
{
	//liczymy dla pierwszego dla MATKI
	
	//gettery
	
	int& M_m=matka->get_mass();
	int& M_o=ojciec->get_mass();
	
	sf::Vector2f& V_m=matka->get_v();
	sf::Vector2f& V_o=ojciec->get_v();
	
	sf::Vector2f& loc_m=matka->get_loc();
	sf::Vector2f& loc_o=ojciec->get_loc();
	
	//konty wyliczałem z relacji pokazanych w https://stackoverflow.com/questions/1736734/circle-circle-collision
	
	float kat_kolizji = 90-(atan(fabs(loc_o.x-loc_m.x)/fabs(loc_o.y-loc_m.y)));
	float kat_przed_matki = atan(V_m.y/V_m.x);
	float kat_przed_ojca = atan(V_o.y/V_o.x);
	
	//NOWE
	
	// zrobione ze wzrotu na https://williamecraver.wixsite.com/elastic-equations
	
	V_m.x = ((V_m.x*cos(kat_przed_matki-kat_kolizji)*((float)M_m-(float)M_o))+2*(float)M_o*V_o.x*cos(kat_przed_ojca-kat_kolizji)/((float)M_m+(float)M_o))*cos(kat_kolizji)+(V_m.x*sin(kat_przed_matki-kat_kolizji)*cos(kat_kolizji+((float)M_PI/2)));
	V_m.y =	((V_m.y*cos(kat_przed_matki-kat_kolizji)*((float)M_m-(float)M_o))+2*(float)M_o*V_o.y*cos(kat_przed_ojca-kat_kolizji)/((float)M_m+(float)M_o))*sin(kat_kolizji)+(V_m.y*sin(kat_przed_matki-kat_kolizji)*sin(kat_kolizji+((float)M_PI/2)));
	V_o.x = ((V_o.x*cos(kat_przed_ojca-kat_kolizji)*((float)M_o-(float)M_m))+2*(float)M_m*V_m.x*cos(kat_przed_matki-kat_kolizji)/((float)M_m+(float)M_o))*cos(kat_kolizji)+(V_o.x*sin(kat_przed_ojca-kat_kolizji)*cos(kat_kolizji+((float)M_PI/2)));
	V_o.y = ((V_o.y*cos(kat_przed_ojca-kat_kolizji)*((float)M_o-(float)M_m))+2*(float)M_m*V_m.y*cos(kat_przed_matki-kat_kolizji)/((float)M_m+(float)M_o))*sin(kat_kolizji)+(V_o.y*sin(kat_przed_ojca-kat_kolizji)*sin(kat_kolizji+((float)M_PI/2)));
	
}

sf::FloatRect Celestial_body::getGlobalBounds()
{
	return wyglond.getGlobalBounds();
}


bool Celestial_body::collision_detec(Celestial_body* CB1, Celestial_body* CB2)
{

	return (distance_from(CB1,CB2)-(CB1->get_radius()+CB2->get_radius())<=0);
}
