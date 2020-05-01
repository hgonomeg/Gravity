#ifndef CELESTIAL_BODY_HPP
#define CELESTIAL_BODY_HPP
#include <SFML/Graphics.hpp>
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
	static std::map<unsigned int, unsigned int> alloc_diagram;
	static std::stack<std::pair<std::map<unsigned int, unsigned int>,unsigned int>> alloc_diagram_stack;
	unsigned int Local_ID;
	unsigned short rc;	
	unsigned int purge;
	sf::Vector2f loc;
	std::vector<sf::Vertex> slad;
	std::list<std::vector<sf::Vertex>>* slady_rodzicow;
	static unsigned int znikacz_sladu; // zmienna do której porównujemy rc

	protected:
	
	sf::Color tracecolor;
	int mass;
	sf::Vector2f v;
	sf::Texture tex;
	sf::CircleShape wyglond;
	float radius;
	bool is_still;
	
	public:
	
	Celestial_body(int mass,const sf::Color& et=sf::Color::White,const sf::Vector2f& location={0,0},const sf::Vector2f& velocity={0,0}); 
	virtual Celestial_body* clone(const Celestial_body&) = 0;
	Celestial_body(const Celestial_body&);
	~Celestial_body();
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override; 
	virtual void draw_trace(sf::RenderTarget& tgt,sf::RenderStates st) const;
	
	mutable std::mutex simultaneity_guardian;

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
	
	static bool collision_detection(const Celestial_body&,const Celestial_body&);
	static const std::map<unsigned int, unsigned int>& get_alloc_diagram();
	static void collision_handle(Celestial_body*, Celestial_body*&); // collisions that merge bodies; perfectly inelastic
	static void bounce_handle(Celestial_body*, Celestial_body*); // perfectly elastic collisions
	static bool change_trace_length(bool);
	static unsigned int get_trace_length();
	static void pushstax();
	static void popstax();
	
	static void set_global_num_of_polygon_sides(unsigned int);
	void set_num_of_polygon_sides(unsigned int);
	
};
#endif