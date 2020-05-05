#ifndef GRAWITACJA_HPP
#define GRAWITACJA_HPP
#include "Simulator.hpp"
#include "Space_objects.hpp"
#include "ResourceLoader.hpp"
#include <string>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <list>
#include <sstream>
#include <iomanip>
#include "Button.hpp"
#include "Textbox.hpp"

enum rendering_quality {  //no. sides in polygons to approximate circles
	minimalist,			//12
	low,				//24
	medium,				//32
	high,				//48
	ultra				//64
};

struct window_translation
{
	bool up;
	bool down;
	bool left;
	bool right;
	window_translation();
};

class UI_state; //the base class to process and store the whole GUI

class UI_tool :public sf::Drawable //abstract class to describe states of the UI (UI tools), contain interactive elements and respond to events
{
	friend class UI_state;
	protected:
	UI_state* parent;

	public:
	
	virtual const std::string& name() = 0;
	virtual bool mouse_button_pressed(sf::Event::MouseButtonEvent&) = 0;
	virtual void mouse_button_released(sf::Event::MouseButtonEvent&) = 0;
	virtual void keyboard_button_pressed(sf::Event::KeyEvent&) = 0;
	virtual void text_entered(sf::Event::TextEvent&) = 0;
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
	virtual void tick() = 0;
};

class UI_masterpanel; //it will inherit from UI_tool

class UI_state :public sf::Drawable 
{
	using sysclck = std::chrono::high_resolution_clock;
	public:
	class hint_text
	{
		sysclck::duration display_time;
		sysclck::time_point init_time;
		
		public:
		int last_vertoffset;
		sf::Text sf_text;
		hint_text(const std::string&,unsigned int); //content and lifetime in milliseconds
		bool should_fade();
		int process_height(int);
	};
	private:
	
	sysclck::time_point last_tick;
	sysclck::time_point rendering_finished_time;
	int fps;
	float draw_vs_total_time_ratio;

	void set_status_text();
	
	std::list<hint_text> hint_texts;
	UI_tool* current_tool; 
	UI_masterpanel* masterpanel; //masterpanel is a UI_tool that is always displayed because it is the foundation of the UI
	int last_ht_winoffset;
	sf::Text status_text;
	Simulator* simulator;
	std::shared_ptr<sf::RenderWindow> target;

 	public:
	
	~UI_state();
	UI_state(Simulator*,std::shared_ptr<sf::RenderWindow>);
	
	bool debug;
	
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
	void switch_tool(UI_tool*); //switch the current UI_tool
	void mouse_button_pressed(sf::Event::MouseButtonEvent&);
	void mouse_button_released(sf::Event::MouseButtonEvent&);
	void keyboard_button_pressed(sf::Event::KeyEvent&);
	void text_entered(sf::Event::TextEvent&);
	void push_hint_text(hint_text&&);
	void tick(); //animate visual components
	void notify_rendered(); //read time from clocks to calculate timing
	int vertoffset_of_last_ht();
	Simulator* get_simulator();
	const UI_tool* get_current_tool();
};

class CB_gen :public UI_tool //Celestial_body_gen
{
	Planet::planetary_classification temp_planet;
	Star::stellar_classification temp_star; 
	sf::Text inscription;
	Button b_mode; //the button that changes the type of  the celestial body
	enum class cb_type :unsigned short
	{
		Planet,
		Star,
		StillStar,
		Asteroid
	} currently_picked;
	
	int current_mass;
	bool active_state; //a body is currently being created
	sf::Vector2f rel_init_pos; //first click position (celestial body creation)
	sf::Vector2f rel_end_pos; //last click position (celestial body creation)
	void add_body();
	void cycle_type();
	unsigned int last_body; //ID of thelast body that was created
	bool was_removed; //if the most recently created body got removed
	protected:
	
	public:
	static const std::string tool_name;
	virtual const std::string& name() override;
	CB_gen();
	virtual bool mouse_button_pressed(sf::Event::MouseButtonEvent&) override;
	virtual void mouse_button_released(sf::Event::MouseButtonEvent&) override;
	virtual void keyboard_button_pressed(sf::Event::KeyEvent&) override;
	virtual void text_entered(sf::Event::TextEvent&) override;
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
	virtual void tick() override;
};

class CB_selector :public UI_tool //Responsible for displaying information about a given celestial body. Capable of deleting celestial bodies
{
	Celestial_body* current_pick;
	std::list<std::unique_ptr<Celestial_body>>::iterator current_pick_iter;
	unsigned int pick_id;
	bool verify_body();
	void pop_body();
	sf::Text inscription;
	protected:
	
	public:
	static const std::string tool_name;
	virtual const std::string& name() override;
	CB_selector();
	virtual bool mouse_button_pressed(sf::Event::MouseButtonEvent&) override;
	virtual void mouse_button_released(sf::Event::MouseButtonEvent&) override;
	virtual void keyboard_button_pressed(sf::Event::KeyEvent&) override;
	virtual void text_entered(sf::Event::TextEvent&) override;
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
	virtual void tick() override;
};

class UI_masterpanel :public UI_tool //The main tool that is always shown in the interface. The main panel
{
	Button b_gen; //choose CB generator
	Button b_sel; //chose CB selector
	Button b_traces; //toggle traces
	Button b_collision; //cycle collision detection
	Button b_deltraces; //delete traces
	Button b_predtraces; //predict traces
	Button b_accuracy_plus;
	Button b_accuracy_minus;
	Button b_speed_plus;
	Button b_speed_minus;
	Button b_debug; //the hidden debug mode
	rendering_quality quality;
	void collision_cycle();
	void quality_cycle();
	protected:
	
	public:
	static const std::string tool_name;
	virtual const std::string& name() override;
	UI_masterpanel();
	virtual bool mouse_button_pressed(sf::Event::MouseButtonEvent&) override;
	virtual void mouse_button_released(sf::Event::MouseButtonEvent&) override;
	virtual void keyboard_button_pressed(sf::Event::KeyEvent&) override;
	virtual void text_entered(sf::Event::TextEvent&) override;
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
	virtual void tick() override;
};
#endif