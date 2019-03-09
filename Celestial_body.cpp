#include "Celestial_body.hpp"

void Celestial_body::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(wyglond,st);
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
	wyglond.setPosition(loc);
}

Celestial_body::Celestial_body(int imass,const sf::Color& kolorek,const sf::Vector2f& iloc,const sf::Vector2f& iv)
{
	mass = imass;
	loc = iloc;
	v = iv;
	radius = mass/10.f;
	wyglond.setRadius(radius);
	wyglond.setPointCount(64);
	wyglond.setOrigin(radius,radius);
	wyglond.setOutlineThickness(0);
	wyglond.setFillColor(kolorek);
	
	Global_ID++;
	Local_ID=Global_ID; 
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

bool Celestial_body::collision_detec(Celestial_body* CB1, Celestial_body* CB2)
{

	return (distance_from(CB1,CB2)-(CB1->get_radius()+CB2->get_radius())<=0);
}
