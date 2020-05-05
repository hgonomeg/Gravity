#include "Textbox.hpp"

Textbox::Textbox()
:frame(sf::Vector2f(100,15)),
contents("NULL",resources->main_font,12)
{
	frame.setFillColor(sf::Color(0,30,250));
	frame.setOutlineColor(sf::Color::White);
	frame.setOutlineThickness(2);
	contents.setFillColor(sf::Color::White);
	active_state = false;
	charfilter = [](char a){return true;};
}

Textbox::Textbox(const std::function<bool(char)> filter)
:Textbox()
{
	charfilter = filter;
}

Textbox::Textbox(const std::function<bool(char)> filter, const std::string& contnt)
:Textbox()
{
	contents.setString(contnt);
	charfilter = filter;
}
Textbox::Textbox(const std::string& contnt)
:Textbox()
{
	contents.setString(contnt);
}

void Textbox::setContent(const std::string& ust)
{
	contents.setString(ust);
}
void Textbox::clear()
{
	contents.setString("");
}
bool Textbox::mouse_button_pressed(sf::Event::MouseButtonEvent& ev)
{
	if(frame.getGlobalBounds().contains(ev.x,ev.y))
	{
		if(ev.button == sf::Mouse::Left)
		{
			active_state = !active_state;
		}
		if(active_state)
			frame.setFillColor(sf::Color(0,85,230));
		else
			frame.setFillColor(sf::Color(0,30,250));
		
		return true;
	}
	return false;
}
void Textbox::keyboard_button_pressed(sf::Event::KeyEvent& ev)
{
	//
}
void Textbox::text_entered(sf::Event::TextEvent& ev)
{
	char new_char = static_cast<char>(ev.unicode);
	if(active_state && charfilter(new_char))
	{
		std::string tmp = contents.getString();
		tmp.push_back(new_char);
		contents.setString(tmp);
	}
}
std::string Textbox::getContent() const
{
	return contents.getString();
}
void Textbox::setFilter(const std::function<bool(char)>& nex)
{
	charfilter = nex;
}
void Textbox::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(frame,st);
	tgt.draw(contents,st);
}
void Textbox::setFillColor(const sf::Color& colex)
{
	frame.setFillColor(colex);
}
void Textbox::setSize(const sf::Vector2f& vec)
{
	frame.setSize(vec);
}
void Textbox::setPosition(const sf::Vector2f& vec)
{
	frame.setPosition(vec);
	contents.setPosition(vec.x+3.f,vec.y+3.f);
}

sf::Vector2f Textbox::getPosition() const
{
	return frame.getPosition();
}
void Textbox::setOutlineColor(const sf::Color& col)
{
	frame.setOutlineColor(col);
}
void Textbox::setOutlineThickness(float thicc)
{
	frame.setOutlineThickness(thicc);
}