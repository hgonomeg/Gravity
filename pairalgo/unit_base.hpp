class node;
#ifndef UNIT_BASE_HPP
#define UNIT_BASE_HPP
#define EXTERNPLEASE
#include "wuxing.hpp"

class node :public sf::Drawable
{
	sf::CircleShape ci;
	public:
	virtual void draw(sf::RenderTarget&,sf::RenderStates) const override;
	node(const sf::Vector2f&);
};



class node_stepper
{
	const std::list<node>& nodes;
	wuxing* patris;
	node_stepper(const std::list<node>&, wuxing*);
};


#endif