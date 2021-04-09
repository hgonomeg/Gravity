#ifndef WUXING_HPP
#define WUXING_HPP
#include "../fonts/resources/Arimo-Regular.hpp"
#include <SFML/Graphics.hpp>

#include "unit_base.hpp"
#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <list>
#include <vector>
#include <utility>
#define _USE_MATH_DEFINES
#include <cmath>

inline sf::Font* global_font;
inline wuxing* global_wuxing;


class animated_line
{
	friend class wuxing;
	//two vertices between which the line is drawn
	sf::Vertex first, second;
	//delta is the per-frame growth of the line towards the desired position
	//the desired position vector stores the target position which the second vertex will eventually reach
	//the first vertex does not change since object's construction
	sf::Vector2f delta, desired;
	//tick for the animation
	//returns true when the animation is complete
	bool tick();
	animated_line(const sf::Vector2f&, const sf::Vector2f&);
};

class wuxing :public sf::Drawable
{
	int point_count, pair_count;
	bool finished;
	
	std::mutex finish_mutex;
	std::thread* animation_thread;
	sf::Vector2u viewport_size;
	std::list<node> nodes;
	std::vector<std::pair<sf::Vertex,sf::Vertex>> fixed_lines;
	std::list<animated_line> animated_lines;
 public:
	std::mutex nodes_mutex;


	virtual void draw(sf::RenderTarget&,sf::RenderStates) const override;
	wuxing(int,sf::Vector2u viewport_size);
	~wuxing();

	void animate();
	bool quit();
	int get_pair_count();
	void layout_nodes();
	void evaluate_pair(const std::list<node>::const_iterator&,const std::list<node>::const_iterator&);
	//this interval is the delay of submitting pairs by a node stepper
	//an appriopriate delay is chosen to best show the algorithm's progression
	std::chrono::milliseconds get_best_interval();

};



#endif

