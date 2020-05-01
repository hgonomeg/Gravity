#include <SFML/Graphics.hpp>
#include <list>
#include <math.h>
#include "Celestial_body.hpp"
#include "Tianche_wrapper.hpp"
#include "Gongshi_wrapper.hpp"
#include <memory>
#include <set>

class Simulator :public sf::Drawable
{
	public:
	enum class collision_approach :unsigned short
	{
		merge = 1,
		bounce,
		mixed
	};
	private:
	

	static const float G; //the gravity constant
	static const unsigned short CA_count; //amount of collision approaches
	static const float overlap_tolerance;
	static float STEPPING_RATE; //depends upon accuracy factor
	static int accuracy_factor;
	static unsigned int tick_rate;
	collision_approach ca;
	bool paused, draw_traces;
	gongshi_wrapper<std::unique_ptr<Celestial_body>> twx;
	
	std::list<std::unique_ptr<Celestial_body>> ciala;
	std::list<std::vector<sf::Vertex>>* predicted_traces;

	public:
	static void change_accuracy(bool);
	static bool change_rate(bool);
	static float get_accuracy();
	static float get_overlap_tolerance();
	static unsigned int get_rate();
	
	void tick();
	void pause(bool);
	bool pause() const;
	void add_body(Celestial_body*);
	void toggle_traces();
	bool are_traces_drawn() const;
	void delete_traces();
	void predict_traces();
	void set_circle_approx_polygon(unsigned int);
	std::size_t size() const;
	std::list<std::vector<sf::Vertex>> get_traces();
	Simulator();
	~Simulator();
	Simulator(const Simulator& sim);
	virtual void draw(sf::RenderTarget&,sf::RenderStates) const override;
	
	std::list<std::unique_ptr<Celestial_body>>::iterator at_pos(const sf::Vector2f&); //in case that nothing is found at the location passed in the argument it returns list::end().
	std::list<std::unique_ptr<Celestial_body>>::iterator erase_body(const std::list<std::unique_ptr<Celestial_body>>::iterator& el);
	void erase_body(unsigned int ID);
	std::list<std::unique_ptr<Celestial_body>>::const_iterator get_end() const; //end of the list with celestial bodies
	std::list<std::unique_ptr<Celestial_body>>::iterator iterator_of(unsigned int ID);
	collision_approach cycle_collision_approach();
};


