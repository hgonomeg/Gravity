//silnik animacji do testów algorytmów obsługujących pary
#include "wuxing.hpp"
#include "tianche.hpp"
#include "sequential.hpp"

void wuxing::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{	
		for(auto x: solidne_linie)
		{
			sf::Vertex line[2] = {x.first,x.second};
			tgt.draw(line,2,sf::Lines,st);
		}
		for(auto x: wannabes)
		{
			sf::Vertex line[2] = {x.first,x.second};
			tgt.draw(line,2,sf::Lines,st);
		}
		for(auto x: nodes) tgt.draw(x,st);
}

wuxing::wuxing(int cpx,sf::Vector2u winsi)
{
	cp=cpx;
	koniec=false;
	winsiz=winsi;
	sf::Vector2f winshi{(float)winsi.x,(float)winsi.y};
	for(int i=0;i<cp;i++)
	{
		nodes.push_back(node({  winshi.x/2.f+((float)sin(i/(float)cp*2*(float)M_PI)*winshi.x*0.45f)  ,  (winshi.y/2.f-10.f)-((float)cos(i/(float)cp*2*(float)M_PI)*winshi.x*0.45f)  }));
	}
	athd=NULL;
}

bool wuxing::quit()
{
	std::unique_lock<std::mutex> wulock(kon_mut);
	{
	return koniec;
	}
}

std::chrono::milliseconds wuxing::get_best_interval()
{
	return std::chrono::milliseconds((long long)(1000/((float)(cp))));
}

void wuxing::animate()
{
	if(athd==NULL)
	{
		auto athd_func = [this](){
			std::unique_lock<std::mutex>* erb;
			node_stepper* ns;
			for(int i=0;i<2&&!quit();i++)
			{
				switch(i)
				{
				case 0:
				ns = new seq_ns(nodes,this);
				break;
				case 1:
				std::this_thread::sleep_for(std::chrono::milliseconds(5000));
				erb = new std::unique_lock<std::mutex>(nod_mut);
				solidne_linie.clear();
				delete erb;
				ns = new tianche(nodes,this);
				break;
				}
				while(!quit()&&(wannabes.size()>0||!ns->finished()))
				{
					erb = new std::unique_lock<std::mutex>(nod_mut);
					
					for(auto i=wannabes.begin();i!=wannabes.end();i++)
					{
						if(i->tick()) 
						{
							solidne_linie.push_back(std::pair<sf::Vertex,sf::Vertex>(i->first,i->second));
							i=wannabes.erase(i);
							i--;
						}
					}
					delete erb;
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
				delete ns;
				
			}
		};
		athd = new std::thread(athd_func);
	}
}

wuxing::~wuxing()
{
	std::unique_lock<std::mutex>* erb;
	erb = new std::unique_lock<std::mutex>(nod_mut);
	koniec=true;
	delete erb;
	if(athd)
	{
	if(athd->joinable()) athd->join();
	delete athd;
	}
}

void wuxing::consider_pair(const std::list<node>::const_iterator& lhs,const std::list<node>::const_iterator& rhs)
{
	std::unique_lock<std::mutex> prl(nod_mut);
	{
		wannabes.push_back(linewannabe(lhs->get_loc(),rhs->get_loc()));
	}
}

bool linewannabe::tick()
{
	second.position+=delta;
	sf::Vector2f ndelta = {(wannabe.x-second.position.x),(wannabe.y-second.position.y)};
	
	return sqrt(ndelta.x*ndelta.x+ndelta.y*ndelta.y)<=5;
}

linewannabe::linewannabe(const sf::Vector2f& jed, const sf::Vector2f& dwa)
{
	first.position=jed;
	first.color=sf::Color::Green;
	second.color=sf::Color::Green;
	wannabe=dwa;
	second.position=jed;
	delta={(dwa.x-jed.x)/20.f,(dwa.y-jed.y)/20.f};
}

int main()
{
	sf::RenderWindow rehn(sf::VideoMode(500,500),"Wuxing");
	sf::Event ev;
	rehn.setFramerateLimit(60);
	std::unique_lock<std::mutex>* erb;
	std::string napisek="INITVALUE";
	int cp=12;
	wu = new wuxing(cp,rehn.getSize());
	fona = new sf::Font; fona->loadFromMemory(arimo.data,arimo.size); //załadowanie czcionki do obiektu. NIE WOLNO DAĆ TEJ LINIJKI PO LoadResources() 
	sf::Text status_text(std::string(napisek),*fona,12); //informacja o ładowaniu gry
	
	auto zrup_napis = [&status_text,&napisek,&cp,&rehn](){
		napisek="Use L ad P to change node count. Press X to animate.\nCurrent points: ";
		napisek+=std::to_string(cp);
		status_text.setString(napisek);
		status_text.setPosition(rehn.getSize().x/2.f-status_text.getLocalBounds().width/2.f,rehn.getSize().y-(status_text.getLocalBounds().height+5)); //wycentrowanie napisu na dole
	};
	zrup_napis();
	
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
			erb = new std::unique_lock<std::mutex>(wu->nod_mut);
			rehn.draw(*wu);
			delete erb;
			rehn.display();
	}
	delete wu;
	return 0;
}