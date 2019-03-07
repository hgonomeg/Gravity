#include "Space_objects.hpp"

Planet::Planet(unsigned int masa_planety,const sf::Vector2f& lokacja,const sf::Vector2f& predkosc)
:Celestial_body(masa_planety,sf::Color::Blue,lokacja,predkosc)
{




}

Star::Star(unsigned int masa_gwiazdy,const sf::Vector2f& lokacja,const sf::Vector2f& predkosc)
:Celestial_body(masa_gwiazdy,sf::Color::Yellow,lokacja,predkosc)
{


}