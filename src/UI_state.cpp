#include "Grawitacja.hpp"

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
	if(current_tool) current_tool->keyboard_button_pressed(ev);
	
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
	const int separ = 20;
	int tmp_ht_h = last_ht_winoffset;
	int orto_ht_h = [this](){
		int orto = (int)((-1.f)*(float)separ*(float)hint_texts.size()/2.f);
		return orto;
		
	}();
	if(last_ht_winoffset!=orto_ht_h)
	{
		auto sgn = [](int a)->int{return a>=0? 1 : -1;};
		int diff = orto_ht_h - last_ht_winoffset;
		last_ht_winoffset+=cbrt(diff);
	}
	for(auto x=hint_texts.begin();x!=hint_texts.end();)
	{
		 if(x->should_fade())
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
		auto why = x->process_height(tmp_ht_h);
		x->sf_text.setPosition((float)(target->getSize().x/2),(float)(target->getSize().y/2+why));
		tmp_ht_h+=separ;
	}
	if(current_tool) 
		current_tool->tick();
	masterpanel -> tick();
	std::chrono::microseconds interval = std::chrono::duration_cast<std::chrono::microseconds>(sysclck::now() - last_tick);
	fps = (int)(1/((double)interval.count()/(double)1000000));
	set_status_text();
	last_tick = sysclck::now();
}

void UI_state::push_hint_text(hint_text&& x)
{
	x.last_vertoffset = vertoffset_of_last_ht()+20;
	hint_texts.push_back(std::forward<hint_text>(x));
}

void UI_state::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	auto pkp = tgt.getView();
	tgt.setView(sf::View({(float)tgt.getSize().x/2.f,(float)tgt.getSize().y/2.f},{(float)tgt.getSize().x,(float)tgt.getSize().y}));
	if(current_tool) 
		current_tool->draw(tgt,st);
	masterpanel->draw(tgt,st);
	for(auto x=hint_texts.begin();x!=hint_texts.end();x++) 
	{
		tgt.draw(x->sf_text,st);
	}
	tgt.draw(status_text,st);
	tgt.setView(pkp);
}

UI_state::hint_text::hint_text(const std::string& tr,unsigned int mss)
{
	init_time = sysclck::now();
	display_time = std::chrono::duration_cast<sysclck::duration>(std::chrono::milliseconds(mss));
	sf_text.setFont(resources->main_font);
	sf_text.setString(tr);
	sf_text.setCharacterSize(15);
	sf_text.setFillColor(sf::Color(0,255,0,200));
	sf_text.setStyle(sf::Text::Regular);
	sf_text.setOutlineThickness(0);
	sf_text.setOrigin(sf_text.getLocalBounds().width/2.f,sf_text.getLocalBounds().height/2.f);
}
bool UI_state::hint_text::should_fade()
{
	return display_time<(sysclck::now() - init_time);
}

int UI_state::hint_text::process_height(int orto)
{
	auto lv = last_vertoffset;
	if(last_vertoffset!=orto)
	{
		auto sgn = [](int a)->int{return a>=0? 1 : -1;};
		int diff = orto - last_vertoffset;
		last_vertoffset+=cbrt(diff);
	}
	return lv;
}

int UI_state::vertoffset_of_last_ht()
{
	if(hint_texts.size()==0) return 0;
	return hint_texts.size()*20;
}

UI_state::UI_state(Simulator* sjm,std::shared_ptr<sf::RenderWindow> xt)
{
	rendering_finished_time = sysclck::now();
	last_tick = sysclck::now();
	status_text.setFont(resources->main_font);
	status_text.setCharacterSize(15);
	status_text.setPosition(5.f,5.f);
	debug = false;
	current_tool = nullptr;
	simulator = sjm;
	switch_tool(new CB_gen);
	last_ht_winoffset = 0;
	
	fps = 0;
	draw_vs_total_time_ratio = 1.f;
	target = std::move(xt);
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
	tmp<<std::setprecision(2)<<"FPS: "<<fps<<" Draw time vs total: "<<std::fixed<<draw_vs_total_time_ratio*100.f<<std::defaultfloat<<"\%  Accuracy: "<<Simulator::get_accuracy()<<"  Sim. rate: "<<Simulator::get_rate()<<"  Body count: "<<simulator->size()<<"  current_toolent tool: "<<current_tool->name();
	status_text.setString(tmp.str());
}

