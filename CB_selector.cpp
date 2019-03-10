#include "Grawitacja.hpp"

const std::string CB_selector::nam="Celestial body selector";
const std::string& CB_selector::name()
{
	return nam;
}

void CB_selector::mbp(sf::Event& ev)
{
	
}
void CB_selector::mbr(sf::Event& ev)
{
	
}
void CB_selector::kbp(sf::Event& ev)
{
	
}
void CB_selector::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	
}

void CB_selector::pop_body()
{
	c_pick = patris->getsim()->erase_body(c_pick);
}

CB_selector::CB_selector()
{
	c_pick = patris->getsim()->at_pos(0,0);
}

