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

        


    };

















#endif