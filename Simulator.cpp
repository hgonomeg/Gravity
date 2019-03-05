#include "Simulator.hpp"

void Simulator::tick()
{
	
}

void Simulator::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	for(auto& x: ciala)
	{
		x.get()->draw(tgt,st);
	}
}

void Simulator::add_body(Celestial_body* koles)
{
	ciala.push_back(std::unique_ptr<Celestial_body>(koles));
}