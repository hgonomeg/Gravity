//implements a wrapper for our Gongshi algorithm for applying a function to pairs of objects simultaneously
#ifndef GONGSHI_WRAPPER_HPP
#define GONGSHI_WRAPPER_HPP

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <vector>
#include <future>
#include <list>

template <typename T>
    class gongshi_wrapper
    {
        using iter_type = typename std::list<T>::iterator;
        using fx_type = std::function<void(const iter_type&,const iter_type&)>;

        std::list<T>& subject_list;
		std::condition_variable thread_sleeper;
		std::vector<std::thread> thread_pool;
		std::queue<std::pair<unsigned int,fx_type>> work_queue; //A queue that holds the tianche multipliers and the function to be applied to each unique pair of objects
		std::mutex queue_mutex;

        bool destruction_pending; 
		std::vector<bool> computation_ready; //corresponds to individual threads
        std::mutex global_state;
        
        void work_thread(unsigned int); 
		bool not_quit();
        
		void cycle_internal_iterator(iter_type&,unsigned int);
		public:
		gongshi_wrapper(std::list<T>&);
		~gongshi_wrapper();
		void async_pairwise_apply(const fx_type&);
    };

template <typename T>
	gongshi_wrapper<T>::gongshi_wrapper(std::list<T>& li)
	:subject_list(li)
	{
		destruction_pending = false;
		unsigned int thread_count = std::thread::hardware_concurrency();
		if(thread_count<2) thread_count = 2;
		for(unsigned i=0;i<thread_count;i++) computation_ready.push_back(true);
		for(unsigned i=0;i<thread_count;i++) thread_pool.push_back(std::thread(&gongshi_wrapper<T>::work_thread,this,i));
	}

template <typename T>
	gongshi_wrapper<T>::~gongshi_wrapper()
	{
		global_state.lock();
		destruction_pending = true;
		global_state.unlock();
		thread_sleeper.notify_all();
		for(auto& x: thread_pool) x.join();
	}

template <typename T>
	bool gongshi_wrapper<T>::not_quit() //whether the work threads should finish and join
	{
		std::lock_guard<std::mutex> lol(global_state);
        {
            return !destruction_pending;
        }
    }

template <typename T>
	void gongshi_wrapper<T>::async_pairwise_apply(const fx_type& fu)
	{
		auto fut = std::async(std::launch::async,[this,fu]{
			
			for(unsigned int i=1;(float)i<=subject_list.size()/2.f;i++) 
				{
                    queue_mutex.lock();
                    work_queue.push(std::pair(i,fu));
                    queue_mutex.unlock();
				}
			
			global_state.lock();
			for(auto i = computation_ready.begin();i!=computation_ready.end();i++)
				*i = false;
			global_state.unlock();
		});

		do{
			thread_sleeper.notify_all(); //prompt the thread pool to start processing the queue
			std::this_thread::yield();
			std::this_thread::yield();
			std::this_thread::yield();
		}while(fut.wait_for(std::chrono::microseconds(100))!=std::future_status::ready);
		fut.get();

		do{
			thread_sleeper.notify_all(); //prompt the thread pool to start processing the queue
			std::this_thread::yield();
			std::this_thread::yield();
			std::this_thread::yield();
		}while([this]{ //while not done
			std::lock_guard<std::mutex> loko(global_state);
			return !std::all_of(computation_ready.begin(),computation_ready.end(),[](bool x){ return x==true;});
		}());
		
	}

template <typename T>
	void gongshi_wrapper<T>::work_thread(unsigned int threadnum)
	{
		std::unique_lock<std::mutex> lok(queue_mutex,std::defer_lock); //prevents deadlock
		while(true)
		{
			lok.lock();

			if(work_queue.empty()) thread_sleeper.wait(lok,[this,threadnum]{ //thread_sleeper.wait(lok,true) <- does not wait
				if(!not_quit()) return true; //don't pause the thread when it is necessary to deconstruct the object
				bool queue_empty = work_queue.empty(); 
				if(queue_empty) { //if the queue is empty, the thread needs to notify the object that it has completed its job
					global_state.lock();
					computation_ready[threadnum] = true;
					global_state.unlock();
				}
				return !queue_empty; //only pause when the queue is empty
				});

			if(!not_quit()) return; //return from the thread if deconstruction is required
			
			unsigned int work_id = work_queue.front().first;
			fx_type function = work_queue.front().second;
			work_queue.pop();
			size_t queue_size = work_queue.size();

			lok.unlock();

            auto determine_pairs = [this,&function](unsigned int l) ->void
            {
				iter_type starting_iter = subject_list.begin(); 
				std::advance(starting_iter,(l-1)); 	//l - position

				iter_type it = starting_iter;
				std::advance(it,1);

				for(; it!=subject_list.end(); ++it)
				{
					function(starting_iter,it);					
				}
            };
			
			determine_pairs(work_id);

			unsigned concurrency = subject_list.size()/2;
			work_id = (2*concurrency)-work_id+1;

			determine_pairs(work_id);

			if(queue_size==0)
			{
				global_state.lock();
				computation_ready[threadnum] = true;
				global_state.unlock();
			}
		}
	}











#endif