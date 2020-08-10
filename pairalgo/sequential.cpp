#include "sequential.hpp"

sequential_node_stepper::sequential_node_stepper(const std::list<node>& nds, wuxing* wx)
:node_stepper(nds,wx)
{
	threads.push_back(std::thread(&sequential_node_stepper::main_action,this));
}

void sequential_node_stepper::main_action()
{
	//iterate to the node before the last one
	for(auto j=nodes.begin(); j!=(--nodes.end()) && !finished(); j++)
	{
		auto i=j; //an iterator that is always one node ahead of j
		i++;
		//iterate to the end
		for(; i!=nodes.end() && !finished(); i++)
		{
			parent->evaluate_pair(j,i);
			//we do not want to rush all the nodes in one millisecond
			std::this_thread::sleep_for(interval);
		}
		
	
	}
	finish_mutex.lock();
	m_finished = true;
	finish_mutex.unlock();
}