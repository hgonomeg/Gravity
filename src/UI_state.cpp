#include "Gravity.hpp"

const int UI_state::hint_text_separation = 20;

void UI_tool::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	//meant to be empty
}

void UI_state::mouse_button_pressed(sf::Event::MouseButtonEvent& ev)
{
	if(!masterpanel -> mouse_button_pressed(ev)) 
		if(current_tool) current_tool->mouse_button_pressed(ev);
}
void UI_state::mouse_button_released(sf::Event::MouseButtonEvent& ev)
{
	if(current_tool) 
		current_tool->mouse_button_released(ev);
}

void UI_state::text_entered(sf::Event::TextEvent& ev)
{
	if(current_tool)
		current_tool->text_entered(ev);
}

void UI_state::keyboard_button_pressed(sf::Event::KeyEvent& ev)
{
	switch(ev.code)
	{
		case sf::Keyboard::G:
		{
			if(dynamic_cast<CB_gen*>(current_tool)==nullptr)
			{
				push_hint_text(UI_state::hint_text("Celestial body generator: Click and swipe to create celestial bodies. Use M to switch between creating planets and stars",1500));
				switch_tool(new CB_gen);
			}
			break;
		}
		case sf::Keyboard::S:
		{
			if(dynamic_cast<CB_selector*>(current_tool)==nullptr)
			{
				push_hint_text(UI_state::hint_text("Celestial body selector: Use E (or X) to edit (or remove) your current_toolent selection.",1500));
				switch_tool(new CB_selector);
			}
			break;
		}
	}
	masterpanel -> keyboard_button_pressed(ev);
	if(current_tool) 
		current_tool->keyboard_button_pressed(ev);
	
}

void UI_state::notify_rendered()
{
	std::chrono::microseconds since_last_draw = std::chrono::duration_cast<std::chrono::microseconds>(sysclck::now() - rendering_finished_time);
	std::chrono::microseconds rendering_time = std::chrono::duration_cast<std::chrono::microseconds>(sysclck::now() - last_tick);
	rendering_finished_time = sysclck::now();
	draw_vs_total_time_ratio = static_cast<float>(rendering_time.count())/since_last_draw.count();
}

void UI_state::tick()
{
	//process disappearing texts
	for(auto x=hint_texts.begin();x!=hint_texts.end();)
	{
		if(!x->should_fade())
			x++; //just skip it
		else
		{
			if(x->sf_text.getFillColor().a == 0) //if completely transparent
			{
				//sets iterator to the element after the one getting erased
				x = hint_texts.erase(x); 
			}
			else //dim the current hint text
			{
				auto color = x->sf_text.getFillColor();
				color.a--;
				x->sf_text.setFillColor(color);
				x++; //advance
			}
		}
	}

	//animate hint text positions
	int desired_height = (-1.f)*(float)hint_text_separation*hint_texts.size()/2.f;
	for(auto &x: hint_texts)
	{
		auto y_offset = x.process_vertical_postion(desired_height);
		x.sf_text.setPosition( (main_window->getSize().x/2.f), //x: window center
								(main_window->getSize().y/2.f+y_offset)); //y: center + offset
		desired_height += hint_text_separation;
	}
	//propagate tick
	if(current_tool) 
		current_tool->tick();
	masterpanel -> tick();

	//timings and clocks
	std::chrono::microseconds interval = std::chrono::duration_cast<std::chrono::microseconds>(sysclck::now() - last_tick);
	fps = (int)(1/((double)interval.count()/(double)1000000));
	set_status_text();
	last_tick = sysclck::now();
}

void UI_state::push_hint_text(hint_text&& x)
{
	x.vertical_offset = vertical_offset_of_last_hint_text()+hint_text_separation;
	hint_texts.push_back(std::forward<hint_text>(x));
}

void UI_state::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	auto original_view = tgt.getView();
	tgt.setView(sf::View( //set a view that will play nicely with rendering of immovable GUI components
						{ //vector of center
						 (float)tgt.getSize().x/2.f,
						 (float)tgt.getSize().y/2.f
						},
						{ //viewport size
					 	 (float)tgt.getSize().x,
						 (float)tgt.getSize().y
						}));
	if(current_tool) 
		current_tool->draw(tgt,st);
	masterpanel->draw(tgt,st);
	for(const auto& x: hint_texts) 
	{
		tgt.draw(x.sf_text,st);
	}
	tgt.draw(status_text,st);
	tgt.setView(original_view);
}

UI_state::hint_text::hint_text(const std::string& text,unsigned int lifetime_ms)
{
	init_time = sysclck::now();
	display_time = std::chrono::duration_cast<sysclck::duration>(std::chrono::milliseconds(lifetime_ms));
	sf_text.setFont(resources->main_font);
	sf_text.setString(text);
	sf_text.setCharacterSize(resources->ui_font_size);
	sf_text.setFillColor(sf::Color(0,255,0,200));
	sf_text.setStyle(sf::Text::Regular);
	sf_text.setOutlineThickness(0);
	//set origin to its' geometrical center
	sf_text.setOrigin(sf_text.getLocalBounds().width/2.f,sf_text.getLocalBounds().height/2.f);
}
bool UI_state::hint_text::should_fade()
{
	//desired display time < the time for which it currently appears on the screen
	return display_time<(sysclck::now() - init_time);
}

int UI_state::hint_text::process_vertical_postion(int desired_offset)
{
	auto original_offset = vertical_offset;
	if(vertical_offset != desired_offset)
	{
		int diff = desired_offset - vertical_offset;
		//this makes the animations look nice
		vertical_offset += cbrt(diff);
	}
	return original_offset;
}

int UI_state::vertical_offset_of_last_hint_text() const
{
	if(hint_texts.size()==0) 
		return 0;
	return hint_texts.back().vertical_offset;
}

UI_state::UI_state(Simulator* sim)
{
	rendering_finished_time = sysclck::now();
	last_tick = sysclck::now();
	status_text.setFont(resources->main_font);
	status_text.setCharacterSize(resources->ui_font_size);
	status_text.setPosition(5.f,5.f); //realtive to window corner
	debug = false;
	current_tool = nullptr;
	simulator = sim;
	switch_tool(new CB_gen);
	
	fps = 0; //just to initialize
	draw_vs_total_time_ratio = 1.f; //just to initialize
	masterpanel = new UI_masterpanel;
	masterpanel -> parent = this;
}

Simulator* UI_state::get_simulator()
{
	return simulator;
}


const UI_tool* UI_state::get_current_tool()
{
	return current_tool;
}

UI_state::~UI_state()
{
	delete current_tool;
	delete masterpanel;
}

void UI_state::switch_tool(UI_tool* ut)
{
	if(current_tool) delete current_tool;
	current_tool = ut;
	current_tool->parent=this;
	set_status_text();
}

void UI_state::set_status_text()
{
	std::stringstream tmp;
	tmp<<std::setprecision(2)<<"FPS: "<<fps<<" Draw time vs total: "<<std::fixed<<draw_vs_total_time_ratio*100.f<<std::defaultfloat<<"\%  Accuracy: "<<Simulator::get_accuracy()<<"  Sim. rate: "<<Simulator::get_rate()<<"  Body count: "<<simulator->size()<<"  current tool: "<<current_tool->name();
	status_text.setString(tmp.str());
}

