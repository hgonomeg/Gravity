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
	koniec=false;
}

bool node_stepper::finished()
{
	std::unique_lock<std::mutex> locc(kon_mut);
	{
	return koniec;
	}
}

node_stepper::~node_stepper()
{
	std::unique_lock<std::mutex>* locc = new std::unique_lock<std::mutex>(kon_mut);
	koniec = true;
	delete locc;
	for(auto& x: thds) x.join();
}