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

class Celestial_body :public sf::Drawable
{
	static unsigned int Global_ID;
	static std::map<unsigned int, unsigned int> alloc_diagram;
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
	
	
	public:
	
	Celestial_body(int,const sf::Color& et=sf::Color::White,const sf::Vector2f& ye={0,0},const sf::Vector2f& ey={0,0}); 
	virtual Celestial_body* clone(const Celestial_body&) = 0;
	Celestial_body(const Celestial_body& CB);
	~Celestial_body();
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override; // "override" upewnienie się nadpisania metody z klasy od której dziedziczymy
	virtual void draw_trace(sf::RenderTarget& tgt,sf::RenderStates st) const;
	
	int& get_mass();
	float& get_radius();
	sf::Vector2f& get_loc();
	sf::Vector2f& get_v();
	unsigned int get_id();
	void refresh();
	sf::FloatRect getGlobalBounds();
	std::list<std::vector<sf::Vertex>> get_traces();
	//ZMIENNE STATYCZNE
	static unsigned int znikacz_sladu; // zmienna do której porównujemy rc
	//FUNKCJE STATYCZNE
	static float distance_from(Celestial_body* CB1, Celestial_body* CB2); //liczenie odległości między dwoma obiektami, jako argumenty przyjmuje wskaźniki do obiektów
	static bool collision_detec(Celestial_body* CB1, Celestial_body* CB2); //detekcja kolizji dwóch CB, jako argumenty przyjmuje wskaźniki do obiektów
	static const std::map<unsigned int, unsigned int>& get_alloc_diagram();
	static void collision_handle(Celestial_body*, Celestial_body*&); //dlaczego tu jest referancja przy jednym wskaźniku a przy drugim nie? // kolizje perfekcyjnie nieelastyczne
	static void bounce_handle(Celestial_body*, Celestial_body*); // kolizje idealnie sprężyste

	
	
	
};
#endif