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
#include "Button.hpp"

class UI_state;

class UI_tool :public sf::Drawable
{
	friend class UI_state;
	protected:
	UI_state* patris;
	
	public:
	static const std::string nam;
	virtual const std::string& name() = 0;
	virtual bool mbp(sf::Event&) = 0;
	virtual void mbr(sf::Event&) = 0;
	virtual void kbp(sf::Event&) = 0;
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
	virtual void tick() = 0;
};

class UI_masterpanel;

class UI_state :public sf::Drawable 
{
	using sysclck = std::chrono::high_resolution_clock;
	public:
	class hint_text
	{
		sysclck::duration data_waznosci;
		sysclck::time_point init_time;
		
		public:
		int last_vertoffset;
		sf::Text sf_text;
		hint_text(const std::string&,unsigned int);
		bool przeterminowane();
		int process_height(int);
	};
	private:
	
	sysclck::time_point last_tick;
	int fps;
	void set_status_text();
	
	std::list<hint_text> hint_texts;
	UI_tool* curr;
	UI_masterpanel* masterpanel;
	int last_ht_winoffset;
	sf::Text* status_text;
	Simulator* sim;
	sf::RenderWindow* target;
 	public:
	
	~UI_state();
	UI_state(Simulator*,sf::RenderWindow*,sf::Text*);
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
	void switch_tool(UI_tool*);
	void mbp(sf::Event&);
	void mbr(sf::Event&);
	void kbp(sf::Event&);
	void push_hint_text(hint_text&&);
	void tick();
	int vertoffset_of_last_ht();
	Simulator* getsim();
	const UI_tool* getcurr();
	sf::RenderWindow* gettgt();
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
	
	int current_mass;
	bool active_state;
	sf::Vector2f rel_init;
	sf::Vector2f rel_end;
	void add_body();
	Celestial_body* lbod;
	protected:
	
	public:
	static const std::string nam;
	virtual const std::string& name() override;
	CB_gen();
	virtual bool mbp(sf::Event&) override;
	virtual void mbr(sf::Event&) override;
	virtual void kbp(sf::Event&) override;
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
	virtual void tick() override;
};

class CB_selector :public UI_tool //Odpowiedzialny za wyświetlanie info o konkretnym ciele niebieskim. Zawiera możlwiość usuwania ciał
{
	Celestial_body* c_pick;
	std::list<std::unique_ptr<Celestial_body>>::iterator c_pick_iter;
	unsigned int pick_id;
	bool verify_body();
	void pop_body();
	sf::Text napis;
	protected:
	
	public:
	static const std::string nam;
	virtual const std::string& name() override;
	CB_selector();
	virtual bool mbp(sf::Event&) override;
	virtual void mbr(sf::Event&) override;
	virtual void kbp(sf::Event&) override;
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
	virtual void tick() override;
};

class UI_masterpanel :public UI_tool //Narzędzie główne należące do UI_state
{
	Button b_gen;
	Button b_sel;
	Button b_traces;
	protected:
	
	public:
	static const std::string nam;
	virtual const std::string& name() override;
	UI_masterpanel();
	virtual bool mbp(sf::Event&) override;
	virtual void mbr(sf::Event&) override;
	virtual void kbp(sf::Event&) override;
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
	virtual void tick() override;
};
#endif