#ifndef TIANCHE_WRAPPER_HPP
#define TIANCHE_WRAPPER_HPP
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <functional>
#include <utility>
#include <queue>
#include <vector>
#include <list>

template <typename T>
	class tianche_wrapper
	{
		void watek();
		std::list<T>& tc;
		std::condition_variable thread_sleeper;
		std::vector<std::thread> thdx;
		std::queue<std::pair<unsigned int,std::function<void(std::list<T>::iterator&,std::list<T>::iterator&)>& > > kolejka;
		std::mutex queue_mutex;
		bool xtime;
		bool not_quit();
		public:
		tianche_wrapper();
		~tianche_wrapper();
		void async_pairwise_apply(const std::function<void(std::list<T>::iterator&,std::list<T>::iterator&)>&);
	};

#endif