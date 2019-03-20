#include "Space_objects.hpp"

Planet::Planet(int masa_planety,const sf::Vector2f& lokacja,const sf::Vector2f& predkosc)
:Celestial_body(masa_planety,sf::Color::Blue,lokacja,predkosc)
{




}

Star::Star(int masa_gwiazdy,const sf::Vector2f& lokacja,const sf::Vector2f& predkosc)
:Celestial_body(masa_gwiazdy,sf::Color::Yellow,lokacja,predkosc)
{


}

Asteroid::Asteroid(const sf::Vector2f& lokacja,const sf::Vector2f& predkosc)
:Celestial_body(1,sf::Color(128,128,128),lokacja,predkosc)
{
	radius = 1.5f;
	wyglond.setOrigin(radius,radius);
	wyglond.setRadius(radius);
}

Spaceship::Spaceship(const sf::Vector2f& lokacja,const sf::Vector2f& predkosc)
:Celestial_body(1,sf::Color::Red,lokacja,predkosc)
{
	tracecolor = sf::Color(255,255,128);
}

Celestial_body* Planet::clone(const Celestial_body& ee)
{
	Celestial_body* ret = new Planet(ee);
	ret->planet_type = ee.planet_type;
	return ret;
}

Celestial_body* Star::clone(const Celestial_body& ee)
{
	Celestial_body* ret = new Star(ee);
	ret->star_type = ee.star_type;
	return ret;
}

Celestial_body* Asteroid::clone(const Celestial_body& ee)
{
	Celestial_body* ret = new Asteroid(ee);
	return ret;
}

Celestial_body* Spaceship::clone(const Celestial_body& ee)
{
	Celestial_body* ret = new Celestial_body(ee);
	Celestial_body& yy = dynamic_cast<Spaceship&>(ee);
	ret->fuel = yy.fuel;
	return ret;
}