#include "Grawitacja.hpp"

const std::string UI_masterpanel::nam="Simulation pace manager";

const std::string& UI_masterpanel::name()
{
	return nam;
}

void UI_masterpanel::mbp(sf::Event& ev)
{
	
}
void UI_masterpanel::mbr(sf::Event& ev)
{
	
}
void UI_masterpanel::kbp(sf::Event& ev)
{
	switch(ev.key.code)
	{
		case sf::Keyboard::L:
		{
			patris->getsim()->toggle_traces();
			break;
		}
	}
}
void UI_masterpanel::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	
}

UI_masterpanel::UI_masterpanel()
{
	
}