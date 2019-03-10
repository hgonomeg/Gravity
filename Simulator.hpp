#include <SFML/Graphics.hpp>
#include <list>
#include <math.h>
#include "Celestial_body.hpp"
#include <memory>

class Simulator :public sf::Drawable
{
	
	static const float G;
	bool paused;
	std::list<std::unique_ptr<Celestial_body>> ciala;
	public:
	
	void tick();
	void pause(bool);
	bool pause();
	
	std::list<std::unique_ptr<Celestial_body>>::iterator at_pos(int,int); //funkcja at_pos(int,int) zwraca iterator listy do elementu który znajduje się na podanych koordynatach. W razie braku ciał niebieskich o podobnych koordynatach, zwraca list::end().
	
	virtual void draw(sf::RenderTarget&,sf::RenderStates) const override;
	
	void add_body(Celestial_body*);
	std::list<std::unique_ptr<Celestial_body>>::iterator erase_body(const std::list<std::unique_ptr<Celestial_body>>::iterator& el);
	
};


