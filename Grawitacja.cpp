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
	switch(ev.key.code)
	{
		case sf::Keyboard::G:
		{
			push_hint_text(hint_text("Celestial body generator: Use M to switch between creating planets and stars",1500));
			switch_tool(new CB_gen);
			break;
		}
		case sf::Keyboard::S:
		{
			push_hint_text(hint_text("Celestial body generator: Use E (or X) to edit (or remove) your current selection.",1500));
			switch_tool(new CB_selector);
			break;
		}
		case sf::Keyboard::T:
		{
			push_hint_text(hint_text("Simulation pace manager",1500));
			switch_tool(new Sim_chrono);
			break;
		}
	}
	if(curr) curr->kbp(ev);
}

void UI_state::tick()
{
	const int separ = 20;
	int tmp_ht_h = last_ht_winoffset;
	int orto_ht_h = [this](){
		int orto = (-1)*separ*hint_texts.size()/2.f;
		return orto;
	}();
	/*
	if(last_ht_winoffset!=orto_ht_h)
	{
		int diff = orto_ht_h - last_ht_winoffset;
		last_ht_winoffset+=cbrt(diff);
	}
	*/
	status_text->setPosition(win->mapPixelToCoords({5,5}));
	status_text->setScale(1/(*scale),1/(*scale));
	for(auto x=hint_texts.begin();x!=hint_texts.end();)
	{
		 if(x->przeterminowane())
		 {
			if(x->sf_text.getFillColor().a==0) x=hint_texts.erase(x);
			else
			{
				auto col = x->sf_text.getFillColor();
				col.a--;
				x->sf_text.setFillColor(col);
				x++;
			}
		 }
		 else x++;
	}
	for(auto x=hint_texts.begin();x!=hint_texts.end();x++)
	{
		x->sf_text.setPosition(win->mapPixelToCoords({(*whatsize).x/2,(*whatsize).y/2+tmp_ht_h}));
		x->sf_text.setScale(1/(*scale),1/(*scale));
		tmp_ht_h+=separ;
	}
}

void UI_state::push_hint_text(hint_text&& x)
{
	hint_texts.push_back(std::forward<hint_text>(x));
}

void UI_state::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	if(curr) curr->draw(tgt,st);
	for(auto x=hint_texts.begin();x!=hint_texts.end();x++) {tgt.draw(x->sf_text,st); std::cout<<(int)(x->sf_text.getFillColor().a)<<'\n';};
	tgt.draw(*status_text,st);
}

UI_state::hint_text::hint_text(const std::string& tr,unsigned int mss)
{
	init_time = sysclck::now();
	data_waznosci = std::chrono::duration_cast<sysclck::duration>(std::chrono::milliseconds(mss));
	sf_text.setFont(*fona);
	sf_text.setString(tr);
	sf_text.setCharacterSize(15);
	sf_text.setFillColor(sf::Color(0,255,0,200));
	sf_text.setStyle(sf::Text::Regular);
	sf_text.setOutlineThickness(0);
	sf_text.setOrigin(sf_text.getLocalBounds().width/2.f,sf_text.getLocalBounds().height/2.f);
}
bool UI_state::hint_text::przeterminowane()
{
	return data_waznosci<(sysclck::now()-init_time);
}

UI_state::UI_state(Simulator* sjm,sf::RenderWindow* xt,sf::Text* stxt,sf::Vector2f* wl,sf::Vector2u* ws, float* sc)
{
	status_text=stxt;
	whatlook = wl;
	whatsize = ws;
	scale = sc;
	curr = NULL;
	switch_tool(new CB_gen);
	sim = sjm;
	win = xt;
	last_ht_winoffset = 0;
	stxt->setCharacterSize(15);
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
	set_status_text();
}

void UI_state::set_status_text()
{
	std::string tmp = "Current tool: ";
	tmp+=curr->name();
	status_text->setString(tmp);
}


int main(int argc, char** argv)
{
	Simulator sim;
	
	sf::Event ev;
	sf::RenderWindow rehn(sf::VideoMode(960,960),"Grawitacja");
	
	rehn.setFramerateLimit(60);
	rehn.setKeyRepeatEnabled(false); //pozwala przyciskom na działanie jako "wciśniętym ciągle" a nie jako serie zdarzeń
	
	sf::Vector2f whatlook(0,0);
	sf::Vector2u whatsize(rehn.getSize());
	float scale=1;
	bool translkeys[4] = {0};
	float translation_constant=30; 

	rehn.clear(); //wypełnienie okna na czarno
	fona = new sf::Font; fona->loadFromMemory(arimo.data,arimo.size); //utworzenie obiektu czcionki
	sf::Text status_text(std::string("Loading..."),*fona); //informacja o ładowaniu gry
	status_text.setPosition(rehn.getSize().x/2.f-status_text.getLocalBounds().width/2.f,rehn.getSize().y/2.f-status_text.getLocalBounds().height/2.f); //wycentrowanie napisu
	rehn.draw(status_text); //renderowanie napisu
	rehn.display(); //zamiana bufora obrazu na karcie graficznej czyli pokazanie tego co wyrenderowane
	//std::this_thread::sleep_for(std::chrono::milliseconds(500)); //czekanie 500 milisekund
	zasoby = LoadResources(); //ładowanie gry
	
	UI_state gui(&sim,&rehn,&status_text,&whatlook,&whatsize,&scale);
	gui.push_hint_text(UI_state::hint_text("Welcome to Grawitacja!",5000));
	
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
