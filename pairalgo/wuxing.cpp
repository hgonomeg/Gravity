//silnik animacji do testów algorytmów obsługujących pary
#include "wuxing.hpp"

void wuxing::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	for(auto x: nodes) tgt.draw(x,st);
}

wuxing::wuxing(int cpx,sf::Vector2u winsi)
{
	cp=cpx;
	winsiz=winsi;
	sf::Vector2f winshi{(float)winsi.x,(float)winsi.y};
	for(int i=0;i<cp;i++)
	{
		nodes.push_back(node({  winshi.x/2.f+((float)sin(i/(float)cp*2*(float)M_PI)*winshi.x*0.45f)  ,  winshi.y/2.f-((float)cos(i/(float)cp*2*(float)M_PI)*winshi.x*0.45f)  }));
	}
}

void wuxing::animate()
{
	
}

int main()
{
	sf::RenderWindow rehn(sf::VideoMode(500,500),"Wuxing");
	sf::Event ev;
	rehn.setFramerateLimit(30);
	
	std::string napisek="Current points: 12";
	int cp=12;
	wu = new wuxing(cp,rehn.getSize());
	fona = new sf::Font; fona->loadFromMemory(arimo.data,arimo.size); //załadowanie czcionki do obiektu. NIE WOLNO DAĆ TEJ LINIJKI PO LoadResources() 
	sf::Text status_text(std::string(napisek),*fona,12); //informacja o ładowaniu gry
	status_text.setPosition(rehn.getSize().x/2.f-status_text.getLocalBounds().width/2.f,rehn.getSize().y-(status_text.getLocalBounds().height+5)); //wycentrowanie napisu na dole
	
	auto zrup_napis = [&status_text,&napisek,&cp](){
		napisek="Current points: ";
		napisek+=std::to_string(cp);
		status_text.setString(napisek);
	};
	
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
					case sf::Event::KeyPressed:
					{
						switch (ev.key.code)
						{
							case sf::Keyboard::P:
							{
								cp++;
								delete wu; wu = new wuxing(cp,rehn.getSize());
								zrup_napis();
								break;
							}
							case sf::Keyboard::L:
							{
								if(cp>2) 
								{
									cp--;
									delete wu; wu = new wuxing(cp,rehn.getSize());
									zrup_napis();
								}
								break;
							}
							case sf::Keyboard::X:
							{
								wu->animate();
								break;
							}
						}
						break;
					}
				}
			}
			
			rehn.clear(sf::Color(255,125,125));
			rehn.draw(status_text);
			rehn.draw(*wu);
			rehn.display();
	}
	delete wu;
	return 0;
}