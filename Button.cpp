#include "Button.hpp"

Button::Button(const sf::Texture& tx)
:frejm({15,15})
{
	graycount = 0;
	frejm.setTexture(&tx,false);
	frejm.setOutlineThickness(2);
	frejm.setOutlineColor(sf::Color::White);
}
bool Button::mbp(sf::Event& ev)
{
	if(frejm.getGlobalBounds().contains({ev.mouseButton.x,ev.mouseButton.y}))
		{
			graycount+=6;
			frejm.setOutlineColor(sf::Color(125,125,125));
			return true;
		}
	return false;
}
void Button::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(frejm);
}
void Button::setPosition(const sf::Vector2f& vc)
{
	frejm.setPosition(vc);
}

void Button::tick()
{
	if(graycount>0)
		{
		graycount--;
		if(graycount==0) frejm.setOutlineColor(sf::Color::White);
		}
}