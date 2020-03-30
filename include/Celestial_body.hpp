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
	static std::map<unsigned int, unsigned int> alloc_diagram;
	static std::stack<std::pair<std::map<unsigned int, unsigned int>,unsigned int>> alloc_diagram_stack;
	unsigned int Local_ID;
	unsigned short rc;	
	unsigned int purge;
	sf::Vector2f loc;
	std::vector<sf::Vertex> slad;
	std::list<std::vector<sf::Vertex>>* slady_rodzicow;
	

	protected:
	
	sf::Color tracecolor;
	int mass;
	sf::Vector2f v;
	sf::Texture tex;
	sf::CircleShape wyglond;
	float radius;
	bool is_still;
	
	public:
	
	Celestial_body(int,const sf::Color& et=sf::Color::White,const sf::Vector2f& ye={0,0},const sf::Vector2f& ey={0,0}); 
	virtual Celestial_body* clone(const Celestial_body&) = 0;
	Celestial_body(const Celestial_body&);
	~Celestial_body();
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override; // "override" upewnienie się nadpisania metody z klasy od której dziedziczymy
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

	void refresh();
	sf::FloatRect getGlobalBounds();
	std::list<std::vector<sf::Vertex>> get_traces();
	void delete_traces();
	float distance_from(const Celestial_body&) const; //liczenie odległości między dwoma obiektami
	//ZMIENNE STATYCZNE
	static unsigned int znikacz_sladu; // zmienna do której porównujemy rc
	//FUNKCJE STATYCZNE
	
	static bool collision_detection(const Celestial_body&,const Celestial_body&); //detekcja kolizji dwóch CB, jako argumenty przyjmuje wskaźniki do obiektów
	static const std::map<unsigned int, unsigned int>& get_alloc_diagram();
	static void collision_handle(Celestial_body*, Celestial_body*&); // kolizje perfekcyjnie nieelastyczne
	static void bounce_handle(Celestial_body*, Celestial_body*); // kolizje idealnie sprężyste
	static void pushstax();
	static void popstax();
	
	
	
};
#endif