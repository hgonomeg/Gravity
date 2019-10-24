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


template <typename T>
	class tianche_wrapper
	{

		using fx_type = std::function<void(typename std::list<T>::iterator&,typename std::list<T>::iterator&)>;
		void watek(unsigned int); //argumentem jest mnoznik wuxinga
		std::list<T>& tc;
		std::condition_variable thread_sleeper;
		std::vector<std::thread> thdx;
		std::queue<std::pair<unsigned int,const fx_type&>> kolejka; //kolejka nie wiem co tu robi. Konflikt z mnoznikiem wuxinga? Przestudowiac wuxing.
		std::mutex queue_mutex;
		std::mutex global_state;
		bool xtime;
		bool not_quit();
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
		for(unsigned i=0;i<aixes;i++) thdx.push_back(std::thread(watek,this,i+1));
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
	void tianche_wrapper<T>::watek(unsigned int jmpnum)
	{
		while(not_quit())
		{
			std::this_thread::sleep_for(std::chrono::seconds(2)); //sample action
			thread_sleeper.wait();
		}
	}

#endif