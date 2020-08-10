#include "unit_base.hpp"
#include "wuxing.hpp"

void node::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(circle,st);
}

sf::Vector2f node::get_location() const
{
	return circle.getPosition(); 
}

node::node(const sf::Vector2f& location)
{
	float rad = 5;
	circle.setRadius(rad);
	circle.setOrigin(rad,rad);
	circle.setFillColor(sf::Color::Cyan);
	circle.setOutlineThickness(1);
	circle.setOutlineColor(sf::Color::Black);
	circle.setPosition(location);
}

node_stepper::node_stepper(const std::list<node>& m_nodes, wuxing* parent)
:nodes(m_nodes)
{
	this->parent = parent;
	m_finished = false;
	interval = parent->get_best_interval();
}

bool node_stepper::finished()
{
	bool buf = parent->quit();
	std::lock_guard<std::mutex> k(finish_mutex);
	{
		if(buf) 
			m_finished = true;
		return m_finished;
	}
}

node_stepper::~node_stepper()
{
	finish_mutex.lock();
	//let other threads know that it is time to go
	m_finished = true;
	finish_mutex.unlock();
	for(auto& x: threads) 
		if(x.joinable()) 
			x.join();
}