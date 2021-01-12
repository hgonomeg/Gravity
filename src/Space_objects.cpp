#include "Space_objects.hpp"

std::optional<std::vector<sf::Texture>> Planet::textures = std::optional<std::vector<sf::Texture>>();
std::optional<std::vector<sf::Texture>> Star::textures = std::optional<std::vector<sf::Texture>>();
std::optional<std::vector<sf::Texture>> Asteroid::textures = std::optional<std::vector<sf::Texture>>();

Planet::Planet(int planet_mass,const sf::Vector2f& location,const sf::Vector2f& velocity)
:Celestial_body(planet_mass,sf::Color::Blue,location,velocity)
{




}

Star::Star(int star_mass,const sf::Vector2f& location,const sf::Vector2f& velocity, bool still)
:Celestial_body(star_mass,sf::Color::Yellow,location,velocity)
{
	is_still = still;

}

Asteroid::Asteroid(const sf::Vector2f& location,const sf::Vector2f& velocity)
:Celestial_body(1,sf::Color(128,128,128),location,velocity)
{
	radius = 1.5f;
	if(Asteroid::textures)
	{
		unsigned short index = Celestial_body::random_number(Asteroid::textures.value().size()-1);
		body_sprite.setTexture(&Asteroid::textures.value()[index]);
	}
		
	body_sprite.setOrigin(radius,radius);
	body_sprite.setRadius(radius);
}

Spaceship::Spaceship(const sf::Vector2f& location,const sf::Vector2f& velocity)
:Celestial_body(1,sf::Color::Red,location,velocity)
{
	trace_color = sf::Color(255,255,128);
}

Spaceship::Spaceship(const Celestial_body& source_body)
:Celestial_body(source_body)
{
	
}

Star::Star(const Celestial_body& source_body)
:Celestial_body(source_body)
{
	
}

Planet::Planet(const Celestial_body& source_body)
:Celestial_body(source_body)
{
	
}

Asteroid::Asteroid(const Celestial_body& source_body)
:Celestial_body(source_body)
{
	
}


Celestial_body* Planet::clone(const Celestial_body& source_body)
{
	Planet* ret = new Planet(source_body);
	const Planet& target_type_source = dynamic_cast<const Planet&>(source_body);
	ret->planet_type = target_type_source.planet_type;
	return ret;
}

Celestial_body* Star::clone(const Celestial_body& source_body)
{
	Star* ret = new Star(source_body);
	const Star& target_type_source = dynamic_cast<const Star&>(source_body);
	ret->star_type = target_type_source.star_type;
	return ret;
}

Celestial_body* Asteroid::clone(const Celestial_body& source_body)
{
	Asteroid* ret = new Asteroid(source_body);
	return ret;
}

Celestial_body* Spaceship::clone(const Celestial_body& source_body)
{
	Spaceship* ret = new Spaceship(source_body);
	const Spaceship& target_type_source = dynamic_cast<const Spaceship&>(source_body);
	ret->fuel = target_type_source.fuel;
	return ret;
}