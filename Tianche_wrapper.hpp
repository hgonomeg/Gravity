#ifndef TIANCHE_WRAPPER_HPP
#define TIANCHE_WRAPPER_HPP
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <functional>
#include <utility>
#include <tuple>
#include <queue>
#include <vector>
#include <list>


#if __cplusplus==201703L
#include <numeric>
using std::gcd;
#else
int gcd(int p1, int p2)
	{
	p1=abs(p1);
	p2=abs(p2);
	while(p1!=p2)
		{
		if(p2>p1)
			p2=p2-p1;
		else
			p1=p1-p2;
		}
	return p1;
	}
#endif	

template <typename T>
	class tianche_wrapper
	{
		using iter_type = typename std::list<T>::iterator;
		using fx_type = std::function<void(iter_type&,iter_type&)>;
		void watek(); 
		std::list<T>& tc;
		std::condition_variable thread_sleeper;
		std::vector<std::thread> thdx;
		std::queue<std::pair<unsigned int,const fx_type&>> kolejka; //Kolejka kojarzaca funkcje z mnoznikami tianche, ktore nalezy obrobic
		std::mutex queue_mutex;
		std::mutex global_state;
		bool xtime;
		bool not_quit();
		void cycle_internal_iterator(iter_type&,unsigned int);
		public:
		tianche_wrapper(std::list<T>&);
		~tianche_wrapper();
		void async_pairwise_apply(const fx_type&);
	};

template <typename T>
	tianche_wrapper<T>::tianche_wrapper(std::list<T>& li)
	:tc(li)
	{
		xtime = false;
		unsigned int aixes = std::thread::hardware_concurrency();
		if(aixes<4) aixes = 4;
		for(unsigned i=0;i<aixes;i++) thdx.push_back(std::thread(&tianche_wrapper<T>::watek,this));
	}


template <typename T>
	tianche_wrapper<T>::~tianche_wrapper()
	{
		global_state.lock();
		xtime = true;
		global_state.unlock();
		thread_sleeper.notify_all();
		for(auto& x: thdx) x.join();
	}

template <typename T>
	bool tianche_wrapper<T>::not_quit()
	{
	std::lock_guard lol(global_state);
		{
			return !xtime;
		}
	}
template <typename T>
	void tianche_wrapper<T>::async_pairwise_apply(const fx_type& fu)
	{
		queue_mutex.lock();
		for(unsigned int i=0;i<thdx.size();i++) kolejka.push(std::pair(i+1,fu));
		queue_mutex.unlock();
		thread_sleeper.notify_all();
	}

template <typename T>
	void tianche_wrapper<T>::cycle_internal_iterator(iter_type& xe,unsigned int delta)
	{
		if(delta!=0)
		{
			for(unsigned int i=0;i<delta;i++)
			{
				if(xe==tc.end()) xe=tc.begin();
				xe++;
			}
		}
		if(xe==tc.end()) xe=tc.begin();
	}

template <typename T>
	void tianche_wrapper<T>::watek()
	{
		std::unique_lock<std::mutex> lok(queue_mutex,std::defer_lock);
		while(not_quit())
		{
			lok.lock();
			thread_sleeper.wait(lok,[this]{return !kolejka.empty();});
			std::pair<unsigned int,const fx_type&> kloc = kolejka.front();
			kolejka.pop();
			lok.unlock();
			unsigned int jmpnum = kloc.first;
			const fx_type& fu = kloc.second;
			
			iter_type curref = tc.begin();

			for(unsigned int razy=0;razy<(unsigned int)gcd((int)nodes.size(),(int)jmpnum);razy++)
			{
				iter_type traveller = curref;
				cycle_internal_iterator(traveller,jmpnum);
				iter_type chaser = curref;

				fu(chaser,traveller); 
				chaser = traveller;
				cycle_internal_iterator(traveller,jmpnum);
				if(jmpnum*2==tc.size()) {curref++; continue; }

				do
				{
					fu(chaser,traveller); 
					chaser = traveller;
					cycle_nodes_iterator(traveller,jmpnum);
				} while (chaser!=curref);
				curref++;
			}
		}
	}

#endif