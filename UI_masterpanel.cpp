#include "Grawitacja.hpp"

const std::string UI_masterpanel::nam="UI default main panel";

const std::string& UI_masterpanel::name()
{
	return nam;
}

bool UI_masterpanel::mbp(sf::Event& ev)
{
	bool bu=false;
	if(b_gen.mbp(ev)) {bu=true; if(dynamic_cast<const CB_gen*>(patris->getcurr())==NULL) {patris->switch_tool(new CB_gen); patris->push_hint_text(UI_state::hint_text("Celestial body generator: Click and swipe to create celestial bodies. Use M to switch between creating planets and stars",1500));}}
	if(b_sel.mbp(ev)) {bu=true; if(dynamic_cast<const CB_selector*>(patris->getcurr())==NULL) {patris->switch_tool(new CB_selector); patris->push_hint_text(UI_state::hint_text("Celestial body selector: Use E (or X) to edit (or remove) your current selection.",1500));}}
	if(b_traces.mbp(ev)) {bu=true; patris->getsim()->toggle_traces();}
	return bu;
}
void UI_masterpanel::mbr(sf::Event& ev)
{
	
}
void UI_masterpanel::kbp(sf::Event& ev)
{
	switch(ev.key.code)
	{
		case sf::Keyboard::L:
		{
			patris->getsim()->toggle_traces();
			break;
		}
	}
}
void UI_masterpanel::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(b_gen,st);
	tgt.draw(b_sel,st);
	tgt.draw(b_traces,st);
}

void UI_masterpanel::tick()
{
	b_traces.tick();
	b_gen.tick();
	b_sel.tick();
}

UI_masterpanel::UI_masterpanel()
:b_gen(zasoby->button_CB_gen),
b_sel(zasoby->button_CB_selector),
b_traces(zasoby->button_toggle_traces)
{
	b_gen.setPosition({5,30});
	b_sel.setPosition({25,30});
	b_traces.setPosition({45,30});
}