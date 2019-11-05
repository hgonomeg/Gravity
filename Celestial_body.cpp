#include "Celestial_body.hpp"
#include "Space_objects.hpp"

unsigned int Celestial_body::Global_ID=0;
unsigned int Celestial_body::znikacz_sladu=5;
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
		if(!is_still&&v.x!=0.f&&v.y!=0.f) slad.emplace_back(sf::Vertex(wyglond.getPosition(),tracecolor));
		if(is_still) {loc=wyglond.getPosition(); v={0,0};}
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
	is_still = false;
	radius = sqrt(fabs(mass/10.f));
	wyglond.setRadius(radius);
	wyglond.setPointCount(64);
	wyglond.setOrigin(radius,radius);
	wyglond.setOutlineThickness(0);
	wyglond.setFillColor(kolorek);
	if(mass<0) wyglond.setFillColor(sf::Color(255,200,255));
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
	is_still = rhs.is_still;
	Local_ID=Global_ID; 
	alloc_diagram.emplace(Local_ID,Local_ID);
	if(rhs.slady_rodzicow)
	{
		slady_rodzicow = new std::list<std::vector<sf::Vertex>>(*rhs.slady_rodzicow);
	}
	else slady_rodzicow=NULL;
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
			dziecko->is_still=matka->is_still;
		}
		else if(asteroida!=NULL)
		{
			dziecko=new Asteroid(matka->get_loc(),V_d);
		}
		else //obiekt będzie planetą
		{
			dziecko=new Planet(M_d,matka->get_loc(),V_d);
			dziecko->is_still=matka->is_still;
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
			dziecko->is_still=ojciec->is_still;
		}
		else if(asteroida!=NULL)
		{
			dziecko=new Asteroid(ojciec->get_loc(),V_d);
		}
		else //obiekt będzie planetą
		{
			dziecko=new Planet(M_d,ojciec->get_loc(),V_d);
			dziecko->is_still=ojciec->is_still;
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
	 // 1. W momencie zderzenia wyznaczamy współrzędne środków zderzających się obiektów i numerujemy te obiekty.
 
    auto poz_matki = matka->get_loc(); //auto to sf::Vector2f
    auto poz_ojca = ojciec->get_loc();
    sf::Vector2f& V1 = matka->get_v(); //referencja bo to będziemy zmieniać
    sf::Vector2f& V2 = ojciec->get_v();
    int m1 = matka->get_mass();
    int m2 = ojciec->get_mass();
 
    // 2. Wyznaczamy wektor r12 różnicy położeń tych ciał.
 
    sf::Vector2f S1S2 = poz_matki-poz_ojca;
    sf::Vector2f S2S1 = poz_ojca-poz_matki;
 
    auto vec_dot_product = [](sf::Vector2f pierwszy, sf::Vector2f drugi) -> float {return (pierwszy.x*drugi.x)+(pierwszy.y*drugi.y);}; //iloczyn skalarny
 
    auto powered_vec_length = [](sf::Vector2f vec) -> float {return vec.x*vec.x+vec.y*vec.y;}; //długość wektora
	
	// 3. Rzutujemy wektor v1 oraz wektor v2 na kierunek wektora r12 (można to zrobić bez użycia równania prostej, można wykorzystać własność iloczynu skalarnego, w ten sposób otrzymamy konkretne współrzędne wektora vC1 oraz vc2 (składowych prędkości wzdłuż prostej łączącej środki kul)).

    auto vec_projection = [&](sf::Vector2f pierwszy, sf::Vector2f drugi) -> sf::Vector2f {return static_cast<float>(vec_dot_product(pierwszy,drugi)/powered_vec_length(drugi))*drugi;}; //pierwszy rzucamy na drugi
 
 
    //rzut V1 i V2
    sf::Vector2f V1c = vec_projection(V1,S1S2);
    sf::Vector2f V2c = vec_projection(V2,S2S1);
 
    //4. Szukamy prostopadłych do tej prostej składowych wektorów v1 i v2, np. poprzez różnicę wektorową: vp1 = v1 - vC1 ; vp2 = v2-vC2.
 
    sf::Vector2f V1p = V1-V1c;
    sf::Vector2f V2p = V2-V2c;
 
    //5. Zderzenie sprężyste zachodzi tak, że składowe vC1 i vC2 przetransformują się wg wzorów opisujących zderzenie centralne (które znaleźliście), natomiast składowe vp1 i vp2 pozostaną bez zmian (gdyby kule miały jedynie składowe vp1 i vp2 (tzn v1 = vp1 oraz v2 = vp2, to kule jedynie "musnęłyby się" bez zmiany prędkości)).
    //transformacja V1c i V2c na U1c i U2c
 
    sf::Vector2f U1c = static_cast<float>((m1-m2)/(m1+m2))*V1c + static_cast<float>((2*m2)/(m1+m2))*V2c; //z wzorów na zderzenie centralne sprężyste
    sf::Vector2f U2c = static_cast<float>((2*m1)/(m1+m2))*V1c + static_cast<float>((m2-m1)/(m1+m2))*V2c;
 
    //6. Wektor prędkości po zderzeniu jest równy u1 = vp1 + uC1  ;  u2 = vp2 + uC2 ; gdzie uC1 i uC2 to przetransformowane wektory vC1 i vC2 wg wzorów opisujących zderzenie sprężyste centralne.
 
    sf::Vector2f U1 = U1c + V1p;
    sf::Vector2f U2 = U2c + V2p;
 
    V1 = U1; //ostateczne przypisabue
    V2 = U2;
}

sf::FloatRect Celestial_body::getGlobalBounds()
{
	return wyglond.getGlobalBounds();
}


bool Celestial_body::collision_detec(Celestial_body* CB1, Celestial_body* CB2)
{

	return (distance_from(CB1,CB2)-(CB1->get_radius()+CB2->get_radius())<=0);
}
