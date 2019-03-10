#include "Celestial_body.hpp"

unsigned int Celestial_body::Global_ID=0;

void Celestial_body::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(wyglond,st);
}

void Celestial_body::draw_trace(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(&slad[0],slad.size(),sf::LineStrip,st);
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

		if(rc==5) rc=0;
		if(rc==0)
		{
			for(auto iter=slad.begin();iter!=slad.end();iter++)
			{
				//if(iter==slad.begin()) std::cout<<(int)iter->color.r<<'\n';
				if(iter->color.r==0) purge++;
				else
				{
					iter->color.r--;
					iter->color.g--;
					iter->color.b--;
				}
				if(purge>1000)
				{
					purge=0;
					slad.erase(slad.begin(),std::find_if(slad.rbegin(),slad.rend(),[](const sf::Vertex& xe){return xe.color.r==0;}).base()); //slad jest wektorem którego usuwamy początek
					break;	
				}
				
					
			}
		}
		rc++;
		slad.emplace_back(sf::Vertex(wyglond.getPosition(),sf::Color(255,255,255)));
		wyglond.setPosition(loc);
}

unsigned int Celestial_body::get_id()
{
	return Local_ID;;
}

Celestial_body::Celestial_body(int imass,const sf::Color& kolorek,const sf::Vector2f& iloc,const sf::Vector2f& iv)
{
	mass = imass;
	loc = iloc;
	v = iv;
	rc = 0;
	purge = 0;
	radius = sqrt(mass);
	wyglond.setRadius(radius);
	wyglond.setPointCount(64);
	wyglond.setOrigin(radius,radius);
	wyglond.setOutlineThickness(0);
	wyglond.setFillColor(kolorek);
	wyglond.setPosition(loc);
	slad.emplace_back(sf::Vertex(loc,sf::Color(255,255,255)));
	Local_ID=Global_ID; 
	Global_ID++;
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

sf::FloatRect Celestial_body::getGlobalBounds()
{
	return wyglond.getGlobalBounds();
}

bool Celestial_body::collision_detec(Celestial_body* CB1, Celestial_body* CB2)
{

	return (distance_from(CB1,CB2)-(CB1->get_radius()+CB2->get_radius())<=0);
}
