#include "Grawitacja.hpp"

const std::string CB_gen::name="Celestial body generator";

void CB_gen::mbp(sf::Event& ev)
{
	
}
void CB_gen::mbr(sf::Event& ev)
{
	
}
void CB_gen::kbp(sf::Event& ev)
{
	
}
void CB_gen::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	
}

CB_gen::CB_gen()
{
	mass_multiplier=1;
	currently_picked = cb_type::Planet;
	temp_planet = Planet::planetary_classification::rock;
	temp_star = Star::stellar_classification::A; 
}