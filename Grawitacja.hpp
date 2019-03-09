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


class UI_state;

class UI_tool :public sf::Drawable
{
	friend class UI_state;
	protected:
	UI_state* patris;
	
	public:
	static const std::string name;
	virtual void mbp(sf::Event&) = 0;
	virtual void mbr(sf::Event&) = 0;
	virtual void kbp(sf::Event&) = 0;
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
};

class UI_state :public sf::Drawable
{
	UI_tool* curr;
	Simulator* sim;
 	public:
	~UI_state();
	UI_state(Simulator*);
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
	void switch_tool(UI_tool*);
	void mbp(sf::Event&);
	void mbr(sf::Event&);
	void kbp(sf::Event&);
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
	static const std::string name;
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
	void pop_body();
	protected:
	
	public:
	static const std::string name;
	CB_selector();
	virtual void mbp(sf::Event&) override;
	virtual void mbr(sf::Event&) override;
	virtual void kbp(sf::Event&) override;
	virtual void draw(sf::RenderTarget& tgt,sf::RenderStates st) const override;
};
#endif