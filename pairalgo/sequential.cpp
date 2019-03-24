#include "sequential.hpp"

seq_ns::seq_ns(const std::list<node>& nds, wuxing* wx)
:node_stepper(nds,wx)
{
	thds.push_back(std::thread(main_action,this));
}

void seq_ns::main_action()
{
	
	std::unique_lock<std::mutex>* locc = new std::unique_lock<std::mutex>(kon_mut);
	koniec = true;
	delete locc;
}