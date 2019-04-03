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
#include <type_traits>
#include <any>

template <typename T>
	class tianche_wrapper
	{

		using fx_type = std::function<std::any(typename std::list<T>::iterator&,typename std::list<T>::iterator&)>;
		void watek();
		std::list<T>& tc;
		std::condition_variable thread_sleeper;
		std::vector<std::thread> thdx;
		std::queue<std::pair<unsigned int,const fx_type&>> kolejka;
		std::mutex queue_mutex;
		bool xtime;
		bool not_quit();
		public:
		tianche_wrapper(std::list<T>&);
		~tianche_wrapper();
		std::list<std::any> async_pairwise_apply(const fx_type&);
	};

template <typename T>
	tianche_wrapper<T>::tianche_wrapper(std::list<T>& li)
	:tc(li)
	{
		
	}

template <typename T>
	tianche_wrapper<T>::~tianche_wrapper()
	{

	}

template <typename T>
	bool tianche_wrapper<T>::not_quit()
	{

	}
template <typename T>
	std::list<std::any> tianche_wrapper<T>::async_pairwise_apply(const fx_type&)
	{
		
	}

#endif