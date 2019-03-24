#include "unit_base.hpp"

void node::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(ci,st);
}

sf::Vector2f node::get_loc() const
{
	return ci.getPosition(); 
}

node::node(const sf::Vector2f& loca)
{
	float rad=5;
	ci.setRadius(rad);
	ci.setOrigin(rad,rad);
	ci.setFillColor(sf::Color::Cyan);
	ci.setOutlineThickness(1);
	ci.setOutlineColor(sf::Color::Black);
	ci.setPosition(loca);
}

node_stepper::node_stepper(const std::list<node>& nds, wuxing* ken)
:nodes(nds)
{
	patris=ken;
}