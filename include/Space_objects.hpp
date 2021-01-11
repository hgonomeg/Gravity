#ifndef SPACE_OBJECTS_HPP

#include "Celestial_body.hpp"
//#include "ResourceLoader.hpp"
#include <optional>
#include <vector>

#define SPACE_OBJECTS_HPP

class Planet :public Celestial_body
{
	
	//Enable texturing in the future

	public:
	
	enum planetry_classification 
	{
		gas,
		liquid,
		rock
	}; 
	private:
	
	planetry_classification planet_type;
	public:
	virtual Celestial_body* clone(const Celestial_body&) override;
	Planet(const Celestial_body&);
	Planet(int mass=100, const sf::Vector2f& location={0,0},const sf::Vector2f& velocity={0,0});
};

class Star :public Celestial_body
{
	
	public:
	enum class stellar_classification :sf::Uint32
	{
		O =0x9bb0ff,
		B =0xaabfff,
		A =0xcad7ff,
		F =0xf8f7ff,
		G =0xfff4ea,
		K =0xffd2a1,
		M =0xffcc6f
	}; //Hertzsprung–Russell's diagram star types will be available in the future
	private:
	stellar_classification star_type;
	
	public:
	Star(const Celestial_body&);
	virtual Celestial_body* clone(const Celestial_body&) override;
	Star(int mass=5000, const sf::Vector2f& location={0,0},const sf::Vector2f& velocity={0,0},bool still=false);
};


class Asteroid :public Celestial_body
{
	public:
	Asteroid(const Celestial_body&);
	virtual Celestial_body* clone(const Celestial_body&) override;
	Asteroid(const sf::Vector2f& location={0,0},const sf::Vector2f& velocity={0,0});

	static std::optional<std::vector<sf::Texture>> textures;

};

class Spaceship :public Celestial_body
{
	unsigned short fuel; //max 10 000
	public:
	Spaceship(const Celestial_body&);
	virtual Celestial_body* clone(const Celestial_body&) override;
	Spaceship(const sf::Vector2f& location={0,0},const sf::Vector2f& velocity={0,0});
};

#endif