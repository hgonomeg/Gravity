#include "Space_objects.hpp"

Planet::Planet(unsigned int masa_planety, const sf::Color& kolor,const sf::Vector2f& lokacja,const sf::Vector2f& predkosc)
:Celestial_body(masa_planety,kolor,lokacja,predkosc)
{




}

Star::Star(unsigned int masa_gwiazdy, const sf::Color& kolor,const sf::Vector2f& lokacja,const sf::Vector2f& predkosc)
:Celestial_body(masa_gwiazdy,kolor,lokacja,predkosc)
{


}