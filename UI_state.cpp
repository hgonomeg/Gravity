#include "Grawitacja.hpp"

void UI_tool::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	//ma byc puste
}

void UI_state::mbp(sf::Event& ev)
{
	if(!masterpanel -> mbp(ev)) if(curr) curr->mbp(ev);
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
			if(dynamic_cast<CB_gen*>(curr)==NULL)
			{
			push_hint_text(hint_text("Celestial body generator: Click and swipe to create celestial bodies. Use M to switch between creating planets and stars",1500));
			switch_tool(new CB_gen);
			}
			break;
		}
		case sf::Keyboard::S:
		{
			if(dynamic_cast<CB_selector*>(curr)==NULL)
			{
			push_hint_text(hint_text("Celestial body selector: Use E (or X) to edit (or remove) your current selection.",1500));
			switch_tool(new CB_selector);
			}
			break;
		}
	}
	masterpanel -> kbp(ev);
	if(curr) curr->kbp(ev);
	
}

void UI_state::tick()
{
	const int separ = 20;
	int tmp_ht_h = last_ht_winoffset;
	int orto_ht_h = [this](){
		int orto = (int)((-1.f)*(float)separ*(float)hint_texts.size()/2.f);
		//std::cout<<orto<<'\n';
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
		auto why = x->process_height(tmp_ht_h);
		x->sf_text.setPosition((float)(target->getSize().x/2),(float)(target->getSize().y/2+why));
		tmp_ht_h+=separ;
	}
	if(curr) curr->tick();
	masterpanel -> tick();
	std::chrono::microseconds interval = std::chrono::duration_cast<std::chrono::microseconds>(sysclck::now() - last_tick);
	fps = (int)(1/((double)interval.count()/1000000.d));
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
	if(curr) curr->draw(tgt,st);
	masterpanel->draw(tgt,st);
	for(auto x=hint_texts.begin();x!=hint_texts.end();x++) {tgt.draw(x->sf_text,st);};
	tgt.draw(*status_text,st);
	tgt.setView(pkp);
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

UI_state::UI_state(Simulator* sjm,sf::RenderWindow* xt,sf::Text* stxt)
{
	status_text=stxt;
	debug=false;
	curr = NULL;
	switch_tool(new CB_gen);
	sim = sjm;
	last_ht_winoffset = 0;
	status_text->setCharacterSize(15);
	status_text->setPosition(5.f,5.f);
	fps = 0;
	target = xt;
	masterpanel = new UI_masterpanel;
	masterpanel -> patris = this;
}

Simulator* UI_state::getsim()
{
	return sim;
}

sf::RenderWindow* UI_state::gettgt()
{
	return target;
}

const UI_tool* UI_state::getcurr()
{
	return curr;
}

UI_state::~UI_state()
{
	delete curr;
	delete masterpanel;
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
	std::string tmp = "FPS: "+std::to_string(fps)+" Current tool: "+curr->name();
	status_text->setString(tmp);
}

