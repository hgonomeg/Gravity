#include "tianche.hpp"


tianche::tianche(const std::list<node>& nds, wuxing* wx)
:node_stepper(nds,wx)
{
	thds.push_back(std::thread(main_action,this));
}

void tianche::cycle_nodes_iterator(std::list<node>::const_iterator& xe, unsigned int delta)
{
	if(delta!=0)
	{
		for(unsigned int i=0;i<delta;i++)
		{
			if(xe==nodes.end()) xe=nodes.begin();
			xe++;
		}
	}
	if(xe==nodes.end()) xe=nodes.begin();
}

void tianche::jump_evaluator(unsigned int jmpnum)
{
	std::list<node>::const_iterator curref = nodes.begin();
	for(unsigned int razy=0;!finished()&&razy<(unsigned int)gcd((int)nodes.size(),(int)jmpnum);razy++)
	{
		std::list<node>::const_iterator traveller=curref;
		cycle_nodes_iterator(traveller,jmpnum);
		std::list<node>::const_iterator chaser=curref;
		
		patris->consider_pair(chaser,traveller);
		chaser = traveller;
		cycle_nodes_iterator(traveller,jmpnum);
		std::this_thread::sleep_for(interval);
		if(jmpnum*2==nodes.size()) {curref++; continue; }
		
		do
		{
			patris->consider_pair(chaser,traveller);
			chaser = traveller;
			cycle_nodes_iterator(traveller,jmpnum);
			std::this_thread::sleep_for(interval);
			//if(jmpnum*2==nodes.size()) break;
		}while(chaser!=curref&&!finished());
		curref++;
	}
}

void tianche::main_action()
{
	for(unsigned int jmpnum=1;(float)jmpnum<=nodes.size()/2.f;jmpnum++)
	{
		thds.push_back(std::thread(jump_evaluator,this,jmpnum));
	}
	auto kontrolnik = thds.begin(); kontrolnik++;
	for(;kontrolnik!=thds.end();kontrolnik++) if(kontrolnik->joinable()) kontrolnik->join();
	kon_mut.lock();
	koniec = true;
	kon_mut.unlock();
}