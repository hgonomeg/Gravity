#include "Space_objects.hpp"

Planet::Planet(int masa_planety,const sf::Vector2f& lokacja,const sf::Vector2f& predkosc)
:Celestial_body(masa_planety,sf::Color::Blue,lokacja,predkosc)
{




}

Star::Star(int masa_gwiazdy,const sf::Vector2f& lokacja,const sf::Vector2f& predkosc, bool still)
:Celestial_body(masa_gwiazdy,sf::Color::Yellow,lokacja,predkosc)
{
	is_still=still;

}

Asteroid::Asteroid(const sf::Vector2f& lokacja,const sf::Vector2f& predkosc)
:Celestial_body(1,sf::Color(128,128,128),lokacja,predkosc)
{
	radius = 1.5f;
	body_sprite.setOrigin(radius,radius);
	body_sprite.setRadius(radius);
}

Spaceship::Spaceship(const sf::Vector2f& lokacja,const sf::Vector2f& predkosc)
:Celestial_body(1,sf::Color::Red,lokacja,predkosc)
{
	trace_color = sf::Color(255,255,128);
}

Spaceship::Spaceship(const Celestial_body& ee)
:Celestial_body(ee)
{
	
}

Star::Star(const Celestial_body& ee)
:Celestial_body(ee)
{
	
}

Planet::Planet(const Celestial_body& ee)
:Celestial_body(ee)
{
	
}

Asteroid::Asteroid(const Celestial_body& ee)
:Celestial_body(ee)
{
	
}


Celestial_body* Planet::clone(const Celestial_body& ee)
{
	Planet* ret = new Planet(ee);
	const Planet& yy = dynamic_cast<const Planet&>(ee);
	ret->planet_type = yy.planet_type;
	return ret;
}

Celestial_body* Star::clone(const Celestial_body& ee)
{
	Star* ret = new Star(ee);
	const Star& yy = dynamic_cast<const Star&>(ee);
	ret->star_type = yy.star_type;
	return ret;
}

Celestial_body* Asteroid::clone(const Celestial_body& ee)
{
	Asteroid* ret = new Asteroid(ee);
	return ret;
}

Celestial_body* Spaceship::clone(const Celestial_body& ee)
{
	Spaceship* ret = new Spaceship(ee);
	const Spaceship& yy = dynamic_cast<const Spaceship&>(ee);
	ret->fuel = yy.fuel;
	return ret;
}