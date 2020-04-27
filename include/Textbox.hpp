#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "ResourceLoader.hpp"


class Textbox :public sf::Drawable
{
	sf::RectangleShape frejm;
	sf::Text contents;
	std::function<bool(char)> charfilter;
	bool active_state;
	public:
	Textbox();
	void setContent(const std::string&);
	void clear();
	void mbp(sf::Event&);
	void kbp(sf::Event&);
	void txe(sf::Event&);
	std::string getContent();
	void setFilter(const std::function<bool(char)>&);
	virtual void draw(sf::RenderTarget&,sf::RenderStates) const override;
	void setFillColor(const sf::Color&);
	void setSize(const sf::Vector2f&);
	void setPosition(const sf::Vector2f&);
};