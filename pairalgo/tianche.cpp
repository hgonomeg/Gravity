#include "tianche.hpp"


tianche::tianche(const std::list<node>& nds, wuxing* wx)
:node_stepper(nds,wx)
{
	threads.push_back(std::thread(&tianche::main_action,this));
}

void tianche::cycle_nodes_iterator(std::list<node>::const_iterator& xe, unsigned int delta)
{
	if(delta!=0)
	{
		for(unsigned int i=0;i<delta;i++)
		{
			if(xe==nodes.end()) 
				xe=nodes.begin();
			xe++;
		}
	}
	if(xe==nodes.end()) 
		xe=nodes.begin();
}

void tianche::jump_evaluator(std::size_t jumps)
{
	std::list<node>::const_iterator current = nodes.begin();

	for(std::size_t passes = 0; !finished() && passes<gcd(nodes.size(),jumps); passes++)
	{
		std::list<node>::const_iterator roamer = current;
		cycle_nodes_iterator(roamer,jumps); //roamer goes ahead
		std::list<node>::const_iterator chaser = current; //chases the roamer
		
		parent->evaluate_pair(chaser,roamer);
		//advance in the chase
		chaser = roamer;
		cycle_nodes_iterator(roamer,jumps);
		//we do not want to rush all the nodes in one millisecond
		std::this_thread::sleep_for(interval);
		
		//the special case of tianche
		if(jumps*2 == nodes.size()) 
		{
			current++; 
			continue; 
		}
		
		//continue like this after the possibility of a special case is dealt with
		do
		{
			parent->evaluate_pair(chaser,roamer);
			chaser = roamer;
			cycle_nodes_iterator(roamer,jumps);
			std::this_thread::sleep_for(interval);
		}while(chaser != current&&!finished()); //while we haven't made a full circle

		current++; //advance the current starting point for the next pass
	}
}

void tianche::main_action()
{
	//start the tianche threads
	for(std::size_t jumps=1; (float)jumps<=nodes.size()/2.f; jumps++)
	{
		threads.push_back(std::thread(&tianche::jump_evaluator,this,jumps));
	}

	auto thread_watch = threads.begin(); 
	thread_watch++; //omit the main thread
	//this thread will sleep waiting for the job to be finished so that other threads can be joined
	for(;thread_watch != threads.end(); thread_watch++) 
		if(thread_watch->joinable()) 
			thread_watch->join();

	finish_mutex.lock();
	m_finished = true;
	finish_mutex.unlock();
}