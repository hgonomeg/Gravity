#include <SFML/Graphics.hpp>
#include "ResourceLoader.hpp"

class Button :public sf::Drawable
{
	sf::RectangleShape frame;
	bool shown;
	unsigned int animation_ticks;
	public:
	Button(const sf::Texture&);
	bool mouse_button_pressed(sf::Event::MouseButtonEvent&);
	virtual void draw(sf::RenderTarget&,sf::RenderStates) const override;
	void setPosition(const sf::Vector2f&);
	void tick(); //animate button state
	bool is_shown() const;
	void show(bool);
};