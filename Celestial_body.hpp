#include <SFML/Graphics.hpp>

class Celestial_body :public sf::Drawable
{
	int mass;
	sf::Vector2f loc;
	sf::Vector2f v;
	sf::CircleShape wyglond;
	sf::Color thecol;
	public:
	
	Celestial_body(int,const sf::Color& et=sf::Color::White,const sf::Vector2f& ye={0,0},const sf::Vector2f& ey={0,0}); 
	
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override; // "override" upewnienie się nadpisania metody z klasy od której dzidziczymy
	int& get_mass();
	sf::Vector2f& get_loc();
	sf::Vector2f& get_v();
	
	
};