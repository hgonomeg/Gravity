#ifndef GRAVITY_HPP
#define GRAVITY_HPP
#include "Cmake_defines.hpp"
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

enum window_translation
{
	none,
	up,
	down,
	right,
	left
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
 public: //public types
	class hint_text
	{
		sysclck::duration display_time; //the time duration for which the text should be displayed
		sysclck::time_point init_time; //time when the object was created; used to measure time
		
		public:
		int vertical_offset; //last frame's hint text vertical offset (important for animations)
		sf::Text sf_text;
		hint_text(const std::string& content,unsigned int lifetime_ms); 
		bool should_fade(); //returns true when the hint text has expired and should fade to black
		
		//moves the object so that we get a nice animation of texts sliding into their positions
		//returns initial vertical_offset
		int process_vertical_postion(int desired_vertical_offset); 
	};
 private:
	
	static const int hint_text_separation;
	sysclck::time_point last_tick;
	sysclck::time_point rendering_finished_time;
	int fps;
	float draw_vs_total_time_ratio;

	void set_status_text();
	
	std::list<hint_text> hint_texts;
	UI_tool* current_tool; 
	UI_masterpanel* masterpanel; //masterpanel is a UI_tool that is always displayed because it is the foundation of the UI
	sf::Text status_text;
	Simulator* simulator;

 public:
	
	~UI_state();
	UI_state(Simulator*);
	
	bool debug; //debug mode flag
	
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
	void switch_tool(UI_tool*); //switch the current UI_tool
	void mouse_button_pressed(sf::Event::MouseButtonEvent&);
	void mouse_button_released(sf::Event::MouseButtonEvent&);
	void keyboard_button_pressed(sf::Event::KeyEvent&);
	void text_entered(sf::Event::TextEvent&);
	void push_hint_text(hint_text&&);
	void tick(); //animate visual components
	void notify_rendered(); //read time from clocks to calculate timing
	int vertical_offset_of_last_hint_text() const; //utility: new hint texts need to appear below the last one
	
	Simulator* get_simulator();
	const UI_tool* get_current_tool();
};

class CB_gen :public UI_tool //Celestial_body_gen
{
	Planet::planetry_classification temp_planet;
	Star::stellar_classification temp_star; 
	sf::Text inscription;
	Button b_mode; //the button that changes the type of the celestial body
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
	void cycle_type(); //cycle between celeastial body types
	unsigned int last_body; //ID of thelast body that was created
	bool was_removed; //if the most recently created body got removed

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
	//todo: store buttons in a container for more convenient and less tedious management
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
	void collision_cycle(); //cycle between collision handling approaches
	void quality_cycle(); //cycle between rendering qualities

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