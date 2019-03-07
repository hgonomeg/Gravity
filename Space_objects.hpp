#include "Celestial_body.hpp"

class Planet :public Celestial_body
{
	enum planet_classification 
	{
		gas,
		liquid,
		solid
	} planet_type; 
	//W przyszlosci dodac mechanike teksturowania

	public:
	
	Planet(unsigned int mass=10, sf::Vector2f& location={0,0},const sf::Vector2f& velocity={0,0});
};

class Star :public Celestial_body
{
	enum class stellar_classification :sf::Uint32
	{
		O =0x9bb0ff,
		B =0xaabfff,
		A =0xcad7ff,
		F =0xf8f7ff,
		G =0xfff4ea,
		K =0xffd2a1,
		M =0xffcc6f
	} star_type; //tak jak w przypadku planety; zgodnie z diagramem Hertzsprung–Russella będzie można  znieniać kolor czy teksture gwiazdy

	public:
	
	Star(unsigned int mass=100, sf::Vector2f& location={0,0},const sf::Vector2f& velocity={0,0});
};