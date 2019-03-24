//silnik animacji do testów algorytmów obsługujących pary
#include "wuxing.hpp"

void wuxing::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	
		for(auto x: nodes) tgt.draw(x,st);
		for(auto x: solidne_linie)
		{
			sf::Vertex line[2] = {x.first,x.second};
			tgt.draw(line,2,sf::Lines,st);
		}
}

void wuxing::draw_lock()
{
	wulock.lock();
}

void wuxing::draw_unlock()
{
	wulock.unlock();
}

wuxing::wuxing(int cpx,sf::Vector2u winsi)
{
	wulock = std::unique_lock<std::mutex>(nod_mut,std::defer_lock);
	cp=cpx;
	koniec=false;
	winsiz=winsi;
	sf::Vector2f winshi{(float)winsi.x,(float)winsi.y};
	for(int i=0;i<cp;i++)
	{
		nodes.push_back(node({  winshi.x/2.f+((float)sin(i/(float)cp*2*(float)M_PI)*winshi.x*0.45f)  ,  winshi.y/2.f-((float)cos(i/(float)cp*2*(float)M_PI)*winshi.x*0.45f)  }));
	}
	athd=NULL;
}

void wuxing::animate()
{
	if(athd==NULL)
	{
		auto test = [this]()->bool{std::unique_lock<std::mutex> wulock(kon_mut);{return !koniec;}};
		auto athd_func = [this,test](){
			while(test())
			{
				draw_lock();
				for(auto i=wannabes.begin();i!=wannabes.end();i++)
				{
					if(i->tick()) 
					{
						solidne_linie.push_back(std::pair<sf::Vertex,sf::Vertex>(i->first,i->second));
						i=wannabes.erase(i);
						i--;
					}
				}
				draw_unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}
		};
		athd = new std::thread(athd_func);
	}
}

wuxing::~wuxing()
{
	koniec=true;
	if(athd)
	{
	if(athd->joinable()) athd->join();
	delete athd;
	}
}

void wuxing::consider_pair(const std::list<node>::const_iterator& lhs,const std::list<node>::const_iterator& rhs)
{
	
}

bool linewannabe::tick()
{
	wannabe+=delta;
	return second.position.x-wannabe.x<=0;
}

linewannabe::linewannabe(const sf::Vector2f& jed, const sf::Vector2f& dwa)
{
	first.position=jed;
	first.color=sf::Color::Green;
	second.color=sf::Color::Green;
	wannabe=jed;
	second.position=dwa;
	delta={(dwa.x-jed.x)/60.f,(dwa.y-jed.y)/60.f};
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
			wu->draw_lock();
			rehn.draw(*wu);
			wu->draw_unlock();
			rehn.display();
	}
	delete wu;
	return 0;
}