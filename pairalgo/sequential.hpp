#ifndef SEQUENTIAL_HPP
#define SEQUENTIAL_HPP
#include "wuxing.hpp"
#include "unit_base.hpp"

class sequential_node_stepper :public node_stepper
{
 public:
	sequential_node_stepper(const std::list<node>&, wuxing*);
	virtual void main_action() override;
};

#endif