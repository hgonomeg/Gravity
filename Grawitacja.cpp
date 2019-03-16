#include "Grawitacja.hpp"

using hiresclk = std::chrono::high_resolution_clock;

int main(int argc, char** argv)
{
	hiresclk::time_point init_time = hiresclk::now();
	Simulator sim;
	
	sf::Event ev;
	sf::RenderWindow rehn(sf::VideoMode(960,500),"Grawitacja v0.3.0");
	win = &rehn;
	rehn.setFramerateLimit(60);
	rehn.setKeyRepeatEnabled(false); //pozwala przyciskom na działanie jako "wciśniętym ciągle" a nie jako serie zdarzeń
	
	sf::Vector2f whatlook(0,0);
	whatsize = new sf::Vector2u(rehn.getSize());
	float scale=1;
	bool translkeys[4] = {0};
	float translation_constant=30; 

	rehn.clear(); //wypełnienie okna na czarno
	fona = new sf::Font; fona->loadFromMemory(arimo.data,arimo.size); //załadowanie czcionki do obiektu. NIE WOLNO DAĆ TEJ LINIJKI PO LoadResources() 
	sf::Text status_text(std::string("Loading..."),*fona); //informacja o ładowaniu gry
	status_text.setPosition(rehn.getSize().x/2.f-status_text.getLocalBounds().width/2.f,rehn.getSize().y/2.f-status_text.getLocalBounds().height/2.f); //wycentrowanie napisu
	rehn.draw(status_text); //renderowanie napisu
	rehn.display(); //zamiana bufora obrazu na karcie graficznej czyli pokazanie tego co wyrenderowane
	//std::this_thread::sleep_for(std::chrono::milliseconds(500)); //czekanie 500 milisekund
	zasoby = LoadResources(); //ładowanie gry
	if(!zasoby->correct)
	{
		std::string except_text=zasoby->except_text;
		delete zasoby;
		status_text.setString(std::string("Failed loading resources!\n"+except_text));
		status_text.setPosition(rehn.getSize().x/2.f-status_text.getLocalBounds().width/2.f,rehn.getSize().y/2.f-status_text.getLocalBounds().height/2.f);
		for(unsigned i=0;i<500;i++)
		{
		while(rehn.pollEvent(ev)) {if(ev.type==sf::Event::Closed) rehn.close();}
		if(!rehn.isOpen()) break;
		rehn.clear();
		rehn.draw(status_text); 
		rehn.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(20)); 
		}
		delete fona;
		return 1;
	}
	
	UI_state gui(&sim,&rehn,&status_text);
	gui.push_hint_text(UI_state::hint_text("Welcome to Grawitacja!",12000));
	gui.push_hint_text(UI_state::hint_text("Use G (Generator) and S (Selector) to switch between UI tools.",8000));
	gui.push_hint_text(UI_state::hint_text("For more controls press H",10000));
	
	sim.add_body(new Planet(120,{270,270},{-0.6,1.6}));
	sim.add_body(new Planet(100,{250,250},{-1.2,2.4}));
	sim.add_body(new Star(8500,{200,200},{0.1,-0.05}));
	sim.add_body(new Planet(50,{450,5},{0.6,0.9}));
	sim.add_body(new Planet(150,{4,250},{-0.4,-1.8}));
	
	hiresclk::duration load_time = hiresclk::now() - init_time;
	gui.push_hint_text(UI_state::hint_text(std::string("Done loading in "+std::to_string(((double)(std::chrono::duration_cast<std::chrono::microseconds>(load_time).count()))/1000.d)+" milliseconds"),1000));
	
	bool pauza=false;
	
	auto windowsetter = [&](){
		auto tmp = rehn.getView();
		tmp.setSize((*whatsize).x/scale,(*whatsize).y/scale);
		tmp.setCenter(whatlook+sf::Vector2f((*whatsize).x/2.f,(*whatsize).y/2.f));
		rehn.setView(tmp);
	};
	
	auto perform_translation = [&](){
		if(translkeys[0]) //up
		{
			whatlook.y-=translation_constant/scale;
			windowsetter();
			
		}
		if(translkeys[1]) //down
		{
			whatlook.y+=translation_constant/scale;
			windowsetter();
			
		}
		if(translkeys[2]) //right
		{
			whatlook.x+=translation_constant/scale;
			windowsetter();
			
		}
		if(translkeys[3]) //left
		{
			whatlook.x-=translation_constant/scale;
			windowsetter();
		}
	};
	

	
	while(rehn.isOpen())
	{
		while(rehn.pollEvent(ev))
		{
			switch(ev.type)
			{
				case sf::Event::Closed: // okno zamknięte
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
					{
						pauza=!pauza;
						sim.pause(pauza);
						break;
					}
					case sf::Keyboard::Up:
					{
						translkeys[0]=true;
						break;
					}
					case sf::Keyboard::Down:
					{
						translkeys[1]=true;
						break;
					}
					case sf::Keyboard::Right:
					{
						translkeys[2]=true;
						break;
					}
					case sf::Keyboard::Left:
					{
						translkeys[3]=true;
						break;
					}
					case sf::Keyboard::K: //zmniejszenie znikacza
					{
						if(Celestial_body::znikacz_sladu==1)
						{
							gui.push_hint_text(UI_state::hint_text("Minimal trace length reached",3000));
						}
						else
						Celestial_body::znikacz_sladu--;
						break;
					}
					case sf::Keyboard::O: //zwiekszenie znikacza
					{
						Celestial_body::znikacz_sladu++;
						break;
					}
					case sf::Keyboard::H: //zwiekszenie znikacza
					{
						gui.push_hint_text(UI_state::hint_text("CONTROLS",25000));
						gui.push_hint_text(UI_state::hint_text("S - switch to Celestial body selector",25000));
						gui.push_hint_text(UI_state::hint_text("G - switch to Celestial body generator",25000));
						gui.push_hint_text(UI_state::hint_text("O / K - increase / decrease the length of orbital paths",25000));
						gui.push_hint_text(UI_state::hint_text("P - Pause simulation",25000));
						gui.kbp(ev);
						break;
					}
					default: gui.kbp(ev);
					}
					break;
				}
				case sf::Event::Resized:
				{
					whatsize->x = ev.size.width;
					whatsize->y = ev.size.height;
					windowsetter();
					break;
				}
				case sf::Event::MouseWheelScrolled:
				{
					scale*=pow(1.05,ev.mouseWheelScroll.delta);
					windowsetter();
					break;
				}
				case sf::Event::KeyReleased:
				{
					switch(ev.key.code)
					{
					case sf::Keyboard::Up:
					{
						translkeys[0]=false;
						break;
					}
					case sf::Keyboard::Down:
					{
						translkeys[1]=false;
						break;
					}
					case sf::Keyboard::Right:
					{
						translkeys[2]=false;
						break;
					}
					case sf::Keyboard::Left:
					{
						translkeys[3]=false;
						break;
					}
					}
					break;
				}
			}
		}
		perform_translation();
		gui.tick();
		rehn.clear();
		rehn.draw(sim);
		rehn.draw(gui);
		rehn.display();
		sim.tick(); //tutaj będzie symulacja grawitacji (ruch planet)
	}
	
	delete zasoby;
	delete fona;
}
