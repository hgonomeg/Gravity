#include "Grawitacja.hpp"

void UI_tool::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	//ma byc puste
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

void UI_state::tick()
{
	for(auto x=hint_texts.begin();x!=hint_texts.end();)
	{
	 if(x->przeterminowane())
	 {
	 	x=hint_texts.erase(x);
	 }
	 else x++;
	}
}

void UI_state::push_hint_text(hint_text&& x)
{
	hint_texts.push_back(std::forward(x));
}

void UI_state::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	if(curr) curr->draw(tgt,st);
	for(auto x=hint_texts.begin();x!=hint_texts.end();x++) tgt.draw(x->sf_text,st);
	status_text->draw(tgt,st);
}

UI_state::hint_text::hint_text(const std::string& tr,unsigned int mss)
:text(tr),
 sf_text(text,czcionka,15)
{
	init_time = sysclck::now();
	data_waznosci = std::chrono::duration_cast<sysclck::duration>(std::chrono::milliseconds(mss));
	czcionka.loadFromMemory(arimo.data,arimo.size);
	sf_text.setColor(sf::Color(0,255,0,128));
	
}
bool UI_state::hint_text::przeterminowane()
{
	return data_waznosci>(sysclck::now()-init_time);
}

UI_state::UI_state(Simulator* sjm,sf::Text* stxt)
{
	status_text=stxt;
	curr = new CB_gen;
	curr->patris=this;
	sim = sjm;
	stxt->setCharacterSize(15);
	stxt->setPosition(5,5);
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
	
	sf::Event ev;
	sf::RenderWindow rehn(sf::VideoMode(960,960),"Grawitacja");
	
	rehn.setFramerateLimit(60);
	rehn.setKeyRepeatEnabled(false);
	
	sf::Vector2f whatlook(0,0);
	sf::Vector2u whatsize(rehn.getSize());
	float scale=1;
	bool translkeys[4] = {0};
	float translation_constant=30;
	
	rehn.clear(); //wypełnienie okna na czarno
	sf::Font arimo_font; arimo_font.loadFromMemory(arimo.data,arimo.size); //utworzenie obiektu czcionki
	sf::Text status_text(std::string("Loading..."),arimo_font); //informacja o ładowaniu gry
	status_text.setPosition(rehn.getSize().x/2.f-status_text.getLocalBounds().width/2.f,rehn.getSize().y/2.f-status_text.getLocalBounds().height/2.f); //wycentrowanie napisu
	rehn.draw(status_text); //narysowanie napisu
	rehn.display(); //zamiana bufora obrazu na karcie graficznej czyli pokazanie tego co wyrenderowane
	//std::this_thread::sleep_for(std::chrono::milliseconds(500)); //czekanie 500 milisekund
	zasoby = LoadResources(); //ładowanie gry
	UI_state gui(&sim);
	
	sim.add_body(new Planet(12,{270,270},{-0.6,1.6}));
	sim.add_body(new Planet(10,{250,250},{-1.2,2.4}));
	sim.add_body(new Star(850,{200,200},{0.1,-0.05}));
	sim.add_body(new Planet(5,{450,5},{0.6,0.9}));
	sim.add_body(new Planet(15,{4,250},{-0.4,-1.8}));

	bool pauza=false;
	
	auto windowsetter = [&](){
		auto tmp = rehn.getView();
		tmp.setSize(whatsize.x/scale,whatsize.y/scale);
		tmp.setCenter(whatlook+sf::Vector2f(whatsize.x/2.f,whatsize.y/2.f));
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
					default: gui.kbp(ev);
					}
					break;
				}
				case sf::Event::Resized:
				{
					whatsize.x = ev.size.width;
					whatsize.y = ev.size.height;
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
					default: gui.kbp(ev);
					}
					break;
				}
			}
		}
		perform_translation();
		rehn.clear();
		rehn.draw(sim);
		rehn.draw(gui);
		rehn.display();
		sim.tick(); //tutaj będzie symulacja grawitacji (ruch planet)
		gui.tick();
	}
	
	delete zasoby;
}
