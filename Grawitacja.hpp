#ifndef GRAWITACJA_HPP
#define GRAWITACJA_HPP
#include "Simulator.hpp"
#include "Space_objects.hpp"
#define EXTERNPLEASE
#include "ResourceLoader.hpp"
#include <string>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <list>

class UI_state;

class UI_tool :public sf::Drawable
{
	friend class UI_state;
	protected:
	UI_state* patris;
	
	public:
	static const std::string nam;
	virtual const std::string& name() = 0;
	virtual void mbp(sf::Event&) = 0;
	virtual void mbr(sf::Event&) = 0;
	virtual void kbp(sf::Event&) = 0;
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
};

class UI_state :public sf::Drawable
{
	friend class CB_gen;
	friend class CB_selector;
	friend class Sim_chrono;
	using sysclck = std::chrono::high_resolution_clock;
	public:
	class hint_text
	{
		sysclck::duration data_waznosci;
		sysclck::time_point init_time;
		public:
		
		sf::Text sf_text;
		hint_text(const std::string&,unsigned int);
		bool przeterminowane();
	};
	private:
	
	void set_status_text();
	
	std::list<hint_text> hint_texts;
	UI_tool* curr;
	int last_ht_winoffset;
	sf::Text* status_text;
	Simulator* sim;
	sf::Vector2f* whatlook;
	sf::Vector2u* whatsize;
	float* scale;
	sf::RenderWindow* win;
 	public:
	
	~UI_state();
	UI_state(Simulator*,sf::RenderWindow*,sf::Text*,sf::Vector2f*,sf::Vector2u*,float*);
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
	void switch_tool(UI_tool*);
	void mbp(sf::Event&);
	void mbr(sf::Event&);
	void kbp(sf::Event&);
	void push_hint_text(hint_text&&);
	void tick();
	Simulator* getsim();
};

class CB_gen :public UI_tool //Celestial_body_gen
{
	Planet::planetary_classification temp_planet;
	Star::stellar_classification temp_star; 
	enum cb_type 
	{
		Planet,
		Star
	} currently_picked;
	protected:
	
	public:
	static const std::string nam;
	virtual const std::string& name() override;
	CB_gen();
	unsigned mass_multiplier;
	virtual void mbp(sf::Event&) override;
	virtual void mbr(sf::Event&) override;
	virtual void kbp(sf::Event&) override;
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
};

class CB_selector :public UI_tool //Odpowiedzialny za wyświetlanie info o konkretnym ciele niebieskim. Zawiera możlwiość usuwania ciał
{
	std::list<std::unique_ptr<Celestial_body>>::iterator c_pick;
	std::list<std::unique_ptr<Celestial_body>>::iterator* c_pick_helper;
	void pop_body();
	protected:
	
	public:
	static const std::string nam;
	virtual const std::string& name() override;
	CB_selector();
	virtual void mbp(sf::Event&) override;
	virtual void mbr(sf::Event&) override;
	virtual void kbp(sf::Event&) override;
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
};

class Sim_chrono :public UI_tool //Simulation pace manager; Odpowiedzialny za sterowanie prędkością symulacji
{
	protected:
	
	public:
	static const std::string nam;
	virtual const std::string& name() override;
	Sim_chrono();
	virtual void mbp(sf::Event&) override;
	virtual void mbr(sf::Event&) override;
	virtual void kbp(sf::Event&) override;
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
};
#endif