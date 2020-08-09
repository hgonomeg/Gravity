#ifndef UNIT_BASE_HPP
#define UNIT_BASE_HPP
#include <list>
#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include <mutex>
#include <thread>

class wuxing;

class node :public sf::Drawable
{
	sf::CircleShape ci;
	public:
	sf::Vector2f get_loc() const;
	virtual void draw(sf::RenderTarget&,sf::RenderStates) const override;
	node(const sf::Vector2f&);
};



class node_stepper
{
	protected:
	const std::list<node>& nodes;
	std::vector<std::thread> thds;
	std::chrono::milliseconds interval;
	wuxing* patris;
	bool koniec;
	std::mutex kon_mut;
	public:
	node_stepper(const std::list<node>&, wuxing*);
	virtual void main_action() = 0;
	bool finished();
	~node_stepper();
};


#endif