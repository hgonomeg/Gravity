#ifndef GONGSHI_WRAPPER_HPP
#define GONGSHI_WRAPPER_HPP

#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <vector>
#include <list>

template <typename T>
    class Gongshi_wrapper
    {
        using iter_type = typename std::list<T>::iterator;
        using fx_type = std::function<void(const iter_type&,const iter_type&)>;

        std::list<T>& subject_list;
		std::condition_variable thread_sleeper;
		std::vector<std::thread> thread_pool;
		std::queue<std::pair<unsigned int,fx_type>> work_queue; //A queue that holds the tianche multipliers and the function to be applied to each unique pair of objects
		std::mutex queue_mutex;


    };

















#endif