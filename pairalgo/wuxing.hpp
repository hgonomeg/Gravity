#include "../resources/Arimo-Regular.hpp"
#include <SFML/Graphics.hpp>
class wuxing;
#ifdef EXTERNPLEASE
extern sf::Font* fona;
extern wuxing* wu;
#else
sf::Font* fona;
wuxing* wu;
#endif
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


#ifndef WUXING_HPP
#define WUXING_HPP

class linewannabe
{
	friend class wuxing;
	sf::Vertex first, second;
	sf::Vector2f delta, wannabe;
	bool tick();
	linewannabe(const sf::Vector2f&, const sf::Vector2f&);
};

class wuxing :public sf::Drawable
{
	int cp;
	bool koniec;
	std::mutex kon_mut;
	std::thread* athd;
	sf::Vector2u winsiz;
	std::list<node> nodes;
	std::vector<std::pair<sf::Vertex,sf::Vertex>> solidne_linie;
	std::list<linewannabe> wannabes;
	public:
	std::mutex nod_mut;
	virtual void draw(sf::RenderTarget&,sf::RenderStates) const override;
	wuxing(int,sf::Vector2u winsi);
	~wuxing();
	void animate();
	bool quit();
	void consider_pair(const std::list<node>::const_iterator&,const std::list<node>::const_iterator&);
	std::chrono::milliseconds get_best_interval();
};



#endif

