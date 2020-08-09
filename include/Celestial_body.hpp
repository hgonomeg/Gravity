#ifndef CELESTIAL_BODY_HPP
#define CELESTIAL_BODY_HPP
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <list>
#include <map>
#include <utility>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <stack>

class Celestial_body :public sf::Drawable
{
	static unsigned int Global_ID;
	static unsigned int num_of_polygon_sides;
	static std::map<unsigned int, unsigned int> alloc_diagram; //tracks merge-collisions via mapping ID -> ID  (parent -> child) of merged objects
	static std::stack<std::pair<std::map<unsigned int, unsigned int>,unsigned int>> alloc_diagram_stack;
	
	unsigned int Local_ID; //each celestial body's own unique id
	unsigned short trace_fading_refresh_cycles;	//this counts up to the trace_fading_cycle_interval defined below
	unsigned int trace_trim_counter; //gets incremented when transparent nodes of the trace are detected incicating a need for trimming the trace
	sf::Vector2f location;
	std::vector<sf::Vertex> trace;
	std::list<std::vector<sf::Vertex>>* parents_traces; //stores traces of bodies which collided to produce "this" object
	static unsigned int trace_fading_cycle_interval; // this interval regulates the rate of traces' fading

	protected:
	
	sf::Color trace_color;
	int mass;
	sf::Vector2f velocity;
	sf::Texture texture;
	sf::CircleShape body_sprite;
	float radius;
	bool is_still;
	
	public:
	
	Celestial_body(int mass,const sf::Color& trace_color=sf::Color::White,const sf::Vector2f& location={0,0},const sf::Vector2f& velocity={0,0}); 
	virtual Celestial_body* clone(const Celestial_body&) = 0; //RTTI clone constructor to preserve derived objects' type
	Celestial_body(const Celestial_body&);
	~Celestial_body();
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override; 
	virtual void draw_trace(sf::RenderTarget& tgt,sf::RenderStates st) const;
	
	mutable std::mutex simultaneity_guardian; //for protecting access of parallelized computations

	int get_mass() const;
	float get_radius() const;
	sf::Vector2f get_location() const;
	sf::Vector2f get_velocity() const;
	unsigned int get_id() const;

	void set_mass(int);
	void set_radius(float);
	void set_location(sf::Vector2f);
	void set_velocity(sf::Vector2f);

	void refresh(); //tick fading traces, refresh the position of the graphical shape
	sf::FloatRect getGlobalBounds();
	std::list<std::vector<sf::Vertex>> get_traces();
	void delete_traces();
	float distance_from(const Celestial_body&) const; //calculate distance between bodies
	
	
	//STATIC METHODS
	
	static bool collision_detection(const Celestial_body&,const Celestial_body&); //static method to check if two celestial bodies have collided
	static const std::map<unsigned int, unsigned int>& get_alloc_diagram(); //returns a map describing bodies' collision history
	static void collision_handle(Celestial_body*, Celestial_body*&); // collisions that merge bodies; perfectly inelastic. The second pointer gets overwritten with the child object. The object under the first pointer has to be deleted manually
	static void bounce_handle(Celestial_body*, Celestial_body*); // perfectly elastic collisions
	static bool change_trace_length(bool); //pass 'true' to increase trace length and 'false' to reduce it. Effectively changes the pace of fading of traces
	static unsigned int get_trace_length(); //return the coefficient of pace of trace fading
	
	static void set_global_num_of_polygon_sides(unsigned int);
	void set_num_of_polygon_sides(unsigned int);

	//TO BE REWORKED WHEN NEEDED
	//consider snapshotting static Celestial_body state for better compatibility with the new ideas

	static void pushstax(); //manage the stack of states of alloc_diagram. This will let Simulator to make temporary future trace prediction
	static void popstax(); //manage the stack of states of alloc_diagram. This will let Simulator to make temporary future trace prediction

};
#endif