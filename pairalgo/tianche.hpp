#ifndef TIANCHE_HPP
#define TIANCHE_HPP
#include "unit_base.hpp"

class tianche :public node_stepper
{
	public:
	tianche(const std::list<node>&, wuxing*);
	virtual void main_action() override;
};

#endif