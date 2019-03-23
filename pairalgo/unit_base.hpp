#ifndef UNIT_BASE_HPP
#define UNIT_BASE_HPP
#include <SFML/Graphics.hpp>

class node :public sf::Drawable
{
	sf::CircleShape ci;
	public:
	virtual void draw(sf::RenderTarget&,sf::RenderStates) const override;
	node(const sf::Vector2f&);
};


#endif