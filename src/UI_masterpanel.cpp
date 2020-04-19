#include "Grawitacja.hpp"
#include <sstream>
#include <iomanip>

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
	if(b_collision.mbp(ev)) {bu=true; collision_cycle();}
	if(b_deltraces.mbp(ev)) {bu=true; patris->getsim()->delete_traces(); }
	if(b_predtraces.mbp(ev)) {bu=true; patris->getsim()->predict_traces(); patris->push_hint_text(UI_state::hint_text("Feature currently unimplemented!",3000));}
	if(b_accuracy_plus.mbp(ev)) {bu=true; Simulator::change_accuracy(true);}
	if(b_accuracy_minus.mbp(ev)) {bu=true; Simulator::change_accuracy(false);}
	if(b_speed_plus.mbp(ev)) {bu=true; Simulator::change_rate(true);}
	if(b_speed_minus.mbp(ev)) {bu=true; if(!Simulator::change_rate(false)) patris->push_hint_text(UI_state::hint_text("Minimal simulation rate has been reached!",3000));}
	if(b_debug.mbp(ev)) {bu=true; patris->debug=!patris->debug; b_debug.show(patris->debug); if(patris->debug) patris->push_hint_text(UI_state::hint_text("Debug mode ON",3000)); else patris->push_hint_text(UI_state::hint_text("Debug mode OFF",3000));}
	return bu;
}
void UI_masterpanel::mbr(sf::Event& ev)
{
	
}

void UI_masterpanel::collision_cycle()
{
	Simulator::collision_approach pi =  patris->getsim()->cycle_collision_approach();
	switch(pi)
	{
		case Simulator::collision_approach::merge:
		{
			patris->push_hint_text(UI_state::hint_text("Current collision handling approach: merge",3000));
			break;
		}
		case Simulator::collision_approach::bounce:
		{
			patris->push_hint_text(UI_state::hint_text("Current collision handling approach: bounce",3000));
			break;
		}
		case Simulator::collision_approach::mixed:
		{
			patris->push_hint_text(UI_state::hint_text("Current collision handling approach: mixed",3000));
			patris->push_hint_text(UI_state::hint_text("Current overlap tolerance:"+std::to_string(patris->getsim()->get_overlap_tolerance()),3000)); 
			break;
		}
	}
}

void UI_masterpanel::quality_cycle()
{
	switch(quality)
	{
		case rendering_quality::minimalist:
		{
			patris->push_hint_text(UI_state::hint_text("Current rendering quality: low",1500));
			quality = rendering_quality::low;
			patris->getsim()->set_circle_approx_polygon(24);
			break;
		}
		case rendering_quality::low:
		{
			patris->push_hint_text(UI_state::hint_text("Current rendering quality: medium",1500));
			quality = rendering_quality::medium;
			patris->getsim()->set_circle_approx_polygon(32);
			break;
		}
		case rendering_quality::medium:
		{
			patris->push_hint_text(UI_state::hint_text("Current rendering quality: high",1500));
			quality = rendering_quality::high;
			patris->getsim()->set_circle_approx_polygon(48);
			break;
		}
		case rendering_quality::high:
		{
			patris->push_hint_text(UI_state::hint_text("Current rendering quality: ultra",1500));
			quality = rendering_quality::ultra;
			patris->getsim()->set_circle_approx_polygon(64);
			break;
		}
		case rendering_quality::ultra:
		{
			patris->push_hint_text(UI_state::hint_text("Current rendering quality: minimalist",1500));
			quality = rendering_quality::minimalist;
			patris->getsim()->set_circle_approx_polygon(12);
			break;
		}
	}
}

void UI_masterpanel::kbp(sf::Event& ev)
{
	switch(ev.key.code)
	{
		case sf::Keyboard::L:
		{
			if(!ev.key.control) 
			{
				patris->getsim()->toggle_traces();
				if(patris->getsim()->are_traces_drawn())
					patris->push_hint_text(UI_state::hint_text("Traces enabled",1500));
				else
					patris->push_hint_text(UI_state::hint_text("Traces disabled",1500));
			}
			else 
			{
				patris->getsim()->delete_traces();
				patris->push_hint_text(UI_state::hint_text("Traces deleted",3000));
			}
			break;
		}
		case sf::Keyboard::R:
		{
			collision_cycle();
			break;
		}
		case sf::Keyboard::T:
		{
			patris->getsim()->predict_traces();
			patris->push_hint_text(UI_state::hint_text("Feature currently unimplemented!",3000));
			break;
		}
		case sf::Keyboard::Comma:
		{
			if(!Simulator::change_rate(false)) patris->push_hint_text(UI_state::hint_text("Minimal simulation rate has been reached!",3000));
			break;
		}
		case sf::Keyboard::Period:
		{
			Simulator::change_rate(true);
			break;
		}
		case sf::Keyboard::U:
		{
			Simulator::change_accuracy(false);
			break;
		}
		case sf::Keyboard::I:
		{
			Simulator::change_accuracy(true);
			break;
		}
		case sf::Keyboard::LBracket:
		{
			if(ev.key.control) 
			{
				patris->debug=!patris->debug;
				b_debug.show(patris->debug);
				if(patris->debug) patris->push_hint_text(UI_state::hint_text("Debug mode ON",3000));
				else patris->push_hint_text(UI_state::hint_text("Debug mode OFF",3000));
			}
			else 
				quality_cycle();
			break;
		}
		case sf::Keyboard::H:
		{
			patris->push_hint_text(UI_state::hint_text("L - toggle orbital paths",25000));
			patris->push_hint_text(UI_state::hint_text("R - cycle through available collision handling approaches",25000));
			patris->push_hint_text(UI_state::hint_text("Ctrl+L - delete orbital paths",25000));
			patris->push_hint_text(UI_state::hint_text("T - predict orbital paths (20 seconds)",25000));
			patris->push_hint_text(UI_state::hint_text("U - Decrease the simulation accuracy [also speeds up]",25000));
			patris->push_hint_text(UI_state::hint_text("I - Increase the simulation accuracy [also slows down]",25000));
			patris->push_hint_text(UI_state::hint_text("< - Decrease the simulation rate [Less computations and lower speed]",25000));
			patris->push_hint_text(UI_state::hint_text("> - Increase the simulation rate [More computations and more speed]",25000));
			patris->push_hint_text(UI_state::hint_text("[ - cycle through rendering quality modes",25000));
			break;
		}
	}
}
void UI_masterpanel::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	tgt.draw(b_gen,st);
	tgt.draw(b_sel,st);
	tgt.draw(b_traces,st);
	tgt.draw(b_collision,st);
	tgt.draw(b_deltraces,st);
	tgt.draw(b_predtraces,st);
	tgt.draw(b_accuracy_plus,st);
	tgt.draw(b_accuracy_minus,st);
	tgt.draw(b_speed_plus,st);
	tgt.draw(b_speed_minus,st);
	tgt.draw(b_debug,st);
}

void UI_masterpanel::tick()
{
	b_traces.tick();
	b_gen.tick();
	b_sel.tick();
	b_collision.tick();
	b_deltraces.tick();
	b_predtraces.tick();
	b_accuracy_plus.tick();
	b_accuracy_minus.tick();
	b_speed_plus.tick();
	b_speed_minus.tick();
	b_debug.tick();
}

UI_masterpanel::UI_masterpanel()
:b_gen(zasoby->button_CB_gen),
b_sel(zasoby->button_CB_selector),
b_traces(zasoby->button_toggle_traces),
b_collision(zasoby->button_cycle_collision),
b_deltraces(zasoby->button_deltraces),
b_predtraces(zasoby->button_predtraces),
b_accuracy_plus(zasoby->button_accuracy_plus),
b_accuracy_minus(zasoby->button_accuracy_minus),
b_speed_plus(zasoby->button_speed_plus),
b_speed_minus(zasoby->button_speed_minus),
b_debug(zasoby->button_debug)
{
	b_debug.show(false);
	float x_offset = 5;
	quality = rendering_quality::ultra;
	b_gen.setPosition({x_offset,30}); x_offset+=20.f;
	b_sel.setPosition({x_offset,30}); x_offset+=40.f;
	b_traces.setPosition({x_offset,30}); x_offset+=20.f;
	b_collision.setPosition({x_offset,30}); x_offset+=20.f;
	b_deltraces.setPosition({x_offset,30}); x_offset+=20.f;
	b_predtraces.setPosition({x_offset,30}); x_offset+=40.f;
	b_accuracy_plus.setPosition({x_offset,30}); x_offset+=20.f;
	b_accuracy_minus.setPosition({x_offset,30}); x_offset+=20.f;
	b_speed_plus.setPosition({x_offset,30}); x_offset+=20.f;
	b_speed_minus.setPosition({x_offset,30}); x_offset+=40.f;
	b_debug.setPosition({x_offset,30}); x_offset+=20.f;
}