#include "Simulator.hpp"

const float Simulator::G = 1;

void Simulator::tick()
{
	auto obrob_graw=[this](Celestial_body* lhs, Celestial_body* rhs){
		
		auto &left_loc=lhs->get_loc();
		auto &left_v=lhs->get_v();
		auto &left_mass=lhs->get_mass();
		
		
		auto &right_loc=rhs->get_loc();
		auto &right_v=rhs->get_v();
		auto &right_mass=rhs->get_mass();
		
		float diff_x=left_loc.x-right_loc.x;
		float diff_y=left_loc.y-right_loc.y;
		
		float odleglosc= sqrt((diff_x)*(diff_x)+(diff_y)*(diff_y));
	
		sf::Vector2f sila_graw_vec={diff_x,diff_y};
		sila_graw_vec*=(G*left_mass*right_mass)/(odleglosc*odleglosc*odleglosc);
		
		left_v+=sila_graw_vec;
		right_v-=sila_graw_vec;
		
		
		
	};
	
	for(auto j=ciala.begin(); j!=(--ciala.end()); j++)
	{
		
	
		for(auto i=j; i!=ciala.end(); i++)
		{
			auto iplus = i;
			iplus++;
			obrob_graw(i->get(),iplus->get());
			
		}
		
	
	}
	
	for(auto j=ciala.begin(); j!=ciala.end(); j++)
	{
		auto q=j->get();
		q->get_loc()+(q->get_v());
	}
	
}

void Simulator::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{
	for(auto& x: ciala)
	{
		x.get()->draw(tgt,st);
	}
}

void Simulator::add_body(Celestial_body* koles)
{
	ciala.push_back(std::unique_ptr<Celestial_body>(koles));
}