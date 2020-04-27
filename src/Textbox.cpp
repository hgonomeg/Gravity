#include "Textbox.hpp"

Textbox::Textbox()
:frejm(sf::Vector2f(100,15)),
contents("NULL",resources->main_font,12)
{
	frejm.setFillColor(sf::Color(0,30,250));
	frejm.setOutlineColor(sf::Color::White);
	frejm.setOutlineThickness(2);
	active_state = false;
}
void Textbox::setContent(const std::string& ust)
{
	contents.setString(ust);
}
void Textbox::clear()
{
	contents.setString("");
}
void Textbox::mbp(sf::Event& ev)
{
	
}
void Textbox::kbp(sf::Event& ev)
{
	
}
void Textbox::txe(sf::Event& ev)
{
	
}
std::string Textbox::getContent()
{
	return contents.getString();
}
void Textbox::setFilter(const std::function<bool(char)>& nex)
{
	charfilter = nex;
}
void Textbox::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(frejm,st);
	tgt.draw(contents,st);
}
void Textbox::setFillColor(const sf::Color& colex)
{
	frejm.setFillColor(colex);
}
void Textbox::setSize(const sf::Vector2f& vec)
{
	frejm.setSize(vec);
}
void Textbox::setPosition(const sf::Vector2f& vec)
{
	frejm.setPosition(vec);
	contents.setPosition(vec.x+3.f,vec.y+3.f);
}