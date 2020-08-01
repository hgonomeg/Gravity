#include "Button.hpp"

Button::Button(const sf::Texture& tx)
:frame({15,15})
{
	shown = true;
	animation_ticks = 0;
	frame.setTexture(&tx,false);
	frame.setOutlineThickness(2);
	frame.setOutlineColor(sf::Color::White);
}
bool Button::mouse_button_pressed(sf::Event::MouseButtonEvent& ev)
{
	if(!shown) return false;
	if(frame.getGlobalBounds().contains({static_cast<float>(ev.x),static_cast<float>(ev.y)}))
	{
		animation_ticks+=6;
		frame.setOutlineColor(sf::Color(125,125,125));
		return true;
	}
	return false;
}
void Button::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	if(shown) tgt.draw(frame);
}
void Button::setPosition(const sf::Vector2f& vc)
{
	frame.setPosition(vc);
}

void Button::tick()
{
	if(shown&&animation_ticks>0)
	{
		animation_ticks--;
		if(animation_ticks==0) 
			frame.setOutlineColor(sf::Color::White);
	}
}

bool Button::is_shown() const
{
	return shown;
}

void Button::show(bool ee)
{
	shown = ee;
}