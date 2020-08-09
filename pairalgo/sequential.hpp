#ifndef SEQUENTIAL_HPP
#define SEQUENTIAL_HPP
#include "wuxing.hpp"
#include "unit_base.hpp"

class seq_ns :public node_stepper
{
	public:
	seq_ns(const std::list<node>&, wuxing*);
	virtual void main_action() override;
};

#endif