#include "Celestial_body.hpp"

class Planet :public Celestial_body
{
	enum planet_type {
		
	}; 
	//W przyszlosci dodac mechanike teksturowania

	public:
	
	Planet(unsigned int mass=10, sf::Color& planet_color=sf::Color::Blue, sf::Vector2f& location={0,0},const sf::Vector2f& velocity={0,0});
};

class Star :public Celestial_body
{
	std::string stellar_classification; //tak jak w przypadku planety; zgodnie z diagramem Hertzsprung–Russella będzie można  znieniać kolor czy teksture gwiazdy

	public:
	
	Star(unsigned int mass=100, sf::Color& star_color=sf::Color::Yellow, sf::Vector2f& location={0,0},const sf::Vector2f& velocity={0,0});
};