#include "Grawitacja.hpp"

void UI_tool::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	
}

void UI_state::mbp(sf::Event& ev)
{
	if(curr) curr->mbp(ev);
}
void UI_state::mbr(sf::Event& ev)
{
	if(curr) curr->mbr(ev);
}
void UI_state::kbp(sf::Event& ev)
{
	if(curr) curr->kbp(ev);
}

void UI_state::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	if(curr) curr->draw(tgt,st);
}

UI_state::UI_state(Simulator* sjm)
{
	curr = new CB_gen;
	curr->patris=this;
	sim = sjm;
}

Simulator* UI_state::getsim()
{
	return sim;
}

UI_state::~UI_state()
{
	delete curr;
}

void UI_state::switch_tool(UI_tool* ut)
{
	if(curr) delete curr;
	curr = ut;
	curr->patris=this;
}


int main(int argc, char** argv)
{
	Simulator sim;
	UI_state gui(&sim);
	sf::Event ev;
	sf::RenderWindow rehn(sf::VideoMode(500,500),"Grawitacja");
	rehn.setFramerateLimit(60);
	rehn.clear(); //wypełnienie okna na czarno
	
	sf::Font arimo_font; arimo_font.loadFromMemory(arimo.data,arimo.size); //utworzenie obiektu czcionki
	sf::Text status_text(std::string("Loading..."),arimo_font); //informacja o ładowaniu gry
	status_text.setPosition(rehn.getSize().x/2.f-status_text.getLocalBounds().width/2.f,rehn.getSize().y/2.f-status_text.getLocalBounds().height/2.f); //wycentrowanie napisu
	rehn.draw(status_text); //narysowanie napisu
	rehn.display(); //zamiana bufora obrazu na karcie graficznej czyli pokazanie tego co wyrenderowane
	//std::this_thread::sleep_for(std::chrono::milliseconds(500)); //czekanie 500 milisekund
	zasoby = LoadResources(); //ładowanie gry
	
	sim.add_body(new Planet(10,{250,250},{-0.6,0.8}));
	sim.add_body(new Star(100,{200,200},{0,0}));
	
	bool pauza=false;
	
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
				case sf::Event::MouseButtonPressed:
				{
					gui.mbp(ev);
					break;
				}
				case sf::Event::MouseButtonReleased:
				{
					gui.mbr(ev);
					break;
				}
				case sf::Event::KeyPressed: //zdarzenia wcisniecia klawisza
				{
					switch(ev.key.code)
					{
					case sf::Keyboard::P:
						pauza=!pauza;
						break;
					default: gui.kbp(ev);
					}
					break;
				}

			}
		}
		
		rehn.clear();
		rehn.draw(sim);
		rehn.draw(gui);
		rehn.display();
		if(!pauza) sim.tick(); //tutaj będzie symulacja grawitacji (ruch planet)
	}
	
	delete zasoby;
}
