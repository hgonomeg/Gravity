#include "Gravity.hpp"

using hiresclk = std::chrono::high_resolution_clock;

int main(int argc, char** argv)
{
	hiresclk::time_point init_time = hiresclk::now();
	Simulator sim;
	
	resources.reset(new Resource_Manager());
	main_window.reset(new sf::RenderWindow(sf::VideoMode(960,500),"Gravity v0.4.0"));

	sf::Vector2f canvas_origin(0,0);
	float scale = 1; //scale of the "universe" in the context of the real window size
	window_translation translation_state = none;
	const float translation_constant = 30; 

	//consider config
	main_window->setFramerateLimit(144);
	main_window->setVerticalSyncEnabled(true);
	main_window->setKeyRepeatEnabled(false); //continuous button behavior instead of series of events
	
	//Display a "Loading..." screen while loading resources
	sf::Text status_text(std::string("Loading..."),resources->main_font);
	status_text.setPosition(main_window->getSize().x/2.f-status_text.getLocalBounds().width/2.f,main_window->getSize().y/2.f-status_text.getLocalBounds().height/2.f); //wycentrowanie napisu
	main_window->clear(); 
	main_window->draw(status_text); 
	main_window->display(); 

	try 
	{
		resources->finish_loading();
	}
	catch(std::exception& e)
	{
		std::string except_text(e.what());
		status_text.setString(std::string("Failed loading resources!\n"+except_text));
		status_text.setPosition(main_window->getSize().x/2.f-status_text.getLocalBounds().width/2.f,main_window->getSize().y/2.f-status_text.getLocalBounds().height/2.f); //center the error messagee
		for(unsigned i = 0; i < 500; i++) //just display the message for 500 cycles
		{
			sf::Event ev;
			while(main_window->pollEvent(ev)) 
			{
				if(ev.type==sf::Event::Closed) 
					main_window->close();
			}
			if(!main_window->isOpen()) break;
			main_window->clear();
			main_window->draw(status_text); 
			main_window->display();

		}
		return 1;
	}
	
	//hard-coded celestial bodies displayed at the beginning by default
	sim.add_body(new Planet(120,{270,270},{-0.6,1.6}));
	sim.add_body(new Planet(100,{250,250},{-1.2,2.4}));
	sim.add_body(new Star(8500,{200,200},{0.1,-0.05}));
	sim.add_body(new Planet(50,{450,5},{0.6,0.9}));
	sim.add_body(new Planet(150,{4,250},{-0.4,-1.8}));
	
	UI_state gui(&sim,main_window);
	gui.push_hint_text(UI_state::hint_text("Welcome to Gravity!",12000));
	gui.push_hint_text(UI_state::hint_text("Use G (Generator) and S (Selector) to switch between UI tools.",8000));
	gui.push_hint_text(UI_state::hint_text("For more controls press H",10000));
	
	
	
	hiresclk::duration load_time = hiresclk::now() - init_time;
	gui.push_hint_text(UI_state::hint_text(std::string("Done loading in "+std::to_string(((double)(std::chrono::duration_cast<std::chrono::microseconds>(load_time).count()))/(double)1000)+" milliseconds"),1000));
	
	auto scale_window_canvas = [&](){
		auto tmp = main_window->getView();
		auto win_size = main_window->getSize();
		tmp.setSize(win_size.x/scale,win_size.y/scale);
		tmp.setCenter(canvas_origin+sf::Vector2f(win_size.x/2.f,win_size.y/2.f));
		main_window->setView(tmp);
	};
	
	auto perform_translation = [&](){
		switch(translation_state)
		{
			case up:
			{
				canvas_origin.y-=translation_constant/scale;
				break;
			}
			case down: 
			{
				canvas_origin.y+=translation_constant/scale;
				break;
			}
			case right:
			{
				canvas_origin.x+=translation_constant/scale;
				break;
			}
			case left:
			{
				canvas_origin.x-=translation_constant/scale;
				break;
			}
			default:
			{
				break;
			}
		}
		scale_window_canvas();
	};

	while(main_window->isOpen())
	{
		sf::Event ev;
		while(main_window->pollEvent(ev))
		{
			switch(ev.type)
			{
				case sf::Event::Closed: 
				{
					main_window->close();
					break;
				}
				case sf::Event::MouseButtonPressed:
				{
					gui.mouse_button_pressed(ev.mouseButton);
					break;
				}
				case sf::Event::MouseButtonReleased:
				{
					gui.mouse_button_released(ev.mouseButton);
					break;
				}
				case sf::Event::TextEntered:
				{
					gui.text_entered(ev.text);
					break;
				}
				case sf::Event::KeyPressed: 
				{
					switch(ev.key.code)
					{
						case sf::Keyboard::P:
						{
							sim.pause(!sim.pause());
							break;
						}
						case sf::Keyboard::Up:
						{
							translation_state = up;
							break;
						}
						case sf::Keyboard::Down:
						{
							translation_state = down;
							break;
						}
						case sf::Keyboard::Right:
						{
							translation_state = right;
							break;
						}
						case sf::Keyboard::Left:
						{
							translation_state = left;
							break;
						}
						case sf::Keyboard::K: //increase trace fading rate
						{
							if(!Celestial_body::change_trace_length(false))
								gui.push_hint_text(UI_state::hint_text("Minimal trace length reached",3000));
							else
								gui.push_hint_text(UI_state::hint_text("Trace length factor: "+std::to_string(Celestial_body::get_trace_length()),1500));
							break;
						}
						case sf::Keyboard::O: //decrease trace fading rate
						{
							Celestial_body::change_trace_length(true);
							gui.push_hint_text(UI_state::hint_text("Trace length factor: "+std::to_string(Celestial_body::get_trace_length()),1500));
							break;
						}
						case sf::Keyboard::H:
						{
							gui.push_hint_text(UI_state::hint_text("CONTROLS",25000));
							gui.push_hint_text(UI_state::hint_text("S - switch to Celestial body selector",25000));
							gui.push_hint_text(UI_state::hint_text("G - switch to Celestial body generator",25000));
							gui.push_hint_text(UI_state::hint_text("O / K - increase / decrease the length of orbital paths",25000));
							gui.push_hint_text(UI_state::hint_text("P - Pause simulation",25000));
							gui.keyboard_button_pressed(ev.key);
							break;
						}
						default: 
							gui.keyboard_button_pressed(ev.key);
					}
					break;
				}
				case sf::Event::Resized:
				{
					scale_window_canvas();
					break;
				}
				case sf::Event::MouseWheelScrolled: //zoom control
				{
					scale *= pow(1.05,ev.mouseWheelScroll.delta);
					scale_window_canvas();
					break;
				}
				case sf::Event::KeyReleased:
				{
					switch(ev.key.code)
					{
						case sf::Keyboard::Up:
						case sf::Keyboard::Down:
						case sf::Keyboard::Right:
						case sf::Keyboard::Left:
						{
							translation_state = none;
							break;
						}
						default:
						{
							break;
						}
					}
					break;
				}
			}
		}
		perform_translation();
		gui.tick();
		main_window->clear();
		main_window->draw(sim);
		main_window->draw(gui);
		gui.notify_rendered();
		main_window->display();
		sim.tick();
	}
	

}
