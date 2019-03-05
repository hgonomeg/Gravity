#include <SFML/Graphics.hpp>

class Celestial_body :public sf::Drawable
{
	int mass;
	sf::Vector2f loc;
	sf::Vector2f v;
	sf::CircleShape wyglond;
	sf::Color thecol;
	public:
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
	int get_mass();
	sf::Vector2f& get_loc();
	sf::Vector2f& get_v();
	
	
};