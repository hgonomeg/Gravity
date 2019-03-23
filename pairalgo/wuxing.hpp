#include "unit_base.hpp"
#include "tianche.hpp"
#include "sequential.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <list>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include "../resources/Arimo-Regular.hpp"
sf::Font* fona;

class wuxing :public sf::Drawable
{
	int cp;
	sf::Vector2u winsiz;
	std::list<node> nodes;
	public:
	virtual void draw(sf::RenderTarget&,sf::RenderStates) const override;
	wuxing(int,sf::Vector2u winsi);
	void animate();
};

wuxing* wu;