#include <SFML/Graphics.hpp>
#include <list>
#include "Celestial_body.hpp"
#include <memory>

class Simulator :public sf::Drawable
{
	std::list<std::unique_ptr<Celestial_body>> ciala;
	public:
	
	void tick();
	virtual void draw(sf::RenderTarget&,sf::RenderStates) const override;
	void add_body(Celestial_body*);
	
};