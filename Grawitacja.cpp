#include "Grawitacja.hpp"


int main(int argc, char** argv)
{
	Simulator sim;
	sf::Event ev;
	sf::RenderWindow rehn(sf::VideoMode(500,500),"REHN");
	rehn.setFramerateLimit(60);
	
	while(rehn.isOpen())
	{
		while(rehn.pollEvent(ev))
		{
			switch(ev.type)
			{
				case sf::Event::Closed:
				{
					rehn.close();
					break;
				}
			}
		}
		
		rehn.clear();
		rehn.draw(sim);
		rehn.display();
		sim.tick();
	}
}