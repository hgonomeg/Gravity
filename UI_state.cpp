#include "Grawitacja.hpp"

void UI_tool::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	//ma byc puste
}

void UI_state::mbp(sf::Event& ev)
{
	if(curr) curr->mbp(ev);
	masterpanel -> mbp(ev);
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
			push_hint_text(hint_text("Celestial body generator: Click and swipe to create celestial bodies. Use M to switch between creating planets and stars",1500));
			switch_tool(new CB_gen);
			break;
		}
		case sf::Keyboard::S:
		{
			push_hint_text(hint_text("Celestial body selector: Use E (or X) to edit (or remove) your current selection.",1500));
			switch_tool(new CB_selector);
			break;
		}
	}
	if(curr) curr->kbp(ev);
	masterpanel -> kbp(ev);
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
	std::chrono::microseconds interval = std::chrono::duration_cast<std::chrono::microseconds>(sysclck::now() - last_tick);
	fps = (int)(1/((double)interval.count()/1000000.d));
	set_status_text();
	last_tick = sysclck::now();
}

void UI_state::push_hint_text(hint_text&& x)
{
	hint_texts.push_back(std::forward<hint_text>(x));
}

void UI_state::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	if(curr) curr->draw(tgt,st);
	for(auto x=hint_texts.begin();x!=hint_texts.end();x++) {tgt.draw(x->sf_text,st);};
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
	fps = 0;
	masterpanel = new UI_masterpanel;
	masterpanel -> patris = this;
}

Simulator* UI_state::getsim()
{
	return sim;
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

