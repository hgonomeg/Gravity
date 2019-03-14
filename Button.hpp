#include <SFML/Graphics.hpp>
#define EXTERNPLEASE
#include "ResourceLoader.hpp"

class Button :public sf::Drawable
{
	sf::RectangleShape frejm;
	unsigned int graycount;
	public:
	Button(const sf::Texture&);
	bool mbp(sf::Event&);
	virtual void draw(sf::RenderTarget&,sf::RenderStates) const override;
	void setPosition(const sf::Vector2f&);
	void tick();
};