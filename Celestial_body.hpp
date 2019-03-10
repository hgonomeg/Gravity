#ifndef CELESTIAL_BODY_HPP
#define CELESTIAL_BODY_HPP
#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <algorithm>
#include <iostream>

class Celestial_body :public sf::Drawable
{
	
	
	int mass;
	unsigned short rc;	
	unsigned int purge;
	sf::Vector2f loc;
	sf::Vector2f v;
	sf::CircleShape wyglond;
	sf::Color thecol;
	std::vector<sf::Vertex> slad;
	float radius;
	static unsigned int Global_ID;
	unsigned int Local_ID;
	
	public:
	
	Celestial_body(int,const sf::Color& et=sf::Color::White,const sf::Vector2f& ye={0,0},const sf::Vector2f& ey={0,0}); 
	
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override; // "override" upewnienie się nadpisania metody z klasy od której dziedziczymy
	virtual void draw_trace(sf::RenderTarget& tgt,sf::RenderStates st) const;
	int& get_mass();
	float& get_radius();
	sf::Vector2f& get_loc();
	sf::Vector2f& get_v();
	unsigned int get_id();
	void refresh();
	sf::FloatRect getGlobalBounds();
	static float distance_from(Celestial_body* CB1, Celestial_body* CB2); //liczenie odległości między dwoma obiektami, jako argumenty przyjmuje wskaźniki do obiektów
	static bool collision_detec(Celestial_body* CB1, Celestial_body* CB2); //detekcja kolizji dwóch CB, jako argumenty przyjmuje wskaźniki do obiektów
	
	//zmienne publiczne
	static unsigned int znikacz_sladu; // zmienna do której porównujemy rc
	
};
#endif