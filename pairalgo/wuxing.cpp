//silnik animacji do testów algorytmów obsługujących pary
#include "wuxing.hpp"

int main()
{
	sf::RenderWindow rehn(sf::VideoMode(500,500),"Wuxing");
	sf::Event ev;
	while(rehn.isOpen())
	{
			while(rehn.pollEvent(ev))
			{
				switch(ev.type)
				{
					case sf::Event::Closed:
					{
						rehn.close();
					}
				}
			}
			
			rehn.clear(sf::Color(255,125,125));
			rehn.display();
	}
}