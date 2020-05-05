#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "ResourceLoader.hpp"


class Textbox :public sf::Drawable
{
	sf::RectangleShape frame;
	sf::Text contents;
	std::function<bool(char)> charfilter;
	bool active_state;
	public:
	Textbox();
	Textbox(const std::function<bool(char)>);
	Textbox(const std::string&);
	Textbox(const std::function<bool(char)>, const std::string&);
	void setContent(const std::string&);
	std::string getContent() const;
	void clear();
	void mouse_button_pressed(sf::Event::MouseButtonEvent&);
	void keyboard_button_pressed(sf::Event::KeyEvent&);
	void text_entered(sf::Event::TextEvent&);
	std::string getContent();
	void setFilter(const std::function<bool(char)>&);
	virtual void draw(sf::RenderTarget&,sf::RenderStates) const override;
	void setFillColor(const sf::Color&);
	void setOutlineColor(const sf::Color&);
	void setOutlineThickness(float);
	void setSize(const sf::Vector2f&);
	void setPosition(const sf::Vector2f&);
	sf::Vector2f getPosition() const;
};