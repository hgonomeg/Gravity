
#ifndef RESOURCE_LOADER_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>

#define RESOURCE_LOADER_HPP

struct ResourceBase
{
	bool correct;
};

ResourceBase* LoadResources();

#if defined(EXTERNPLEASE)
ResourceBase* zasoby;
#else
extern ResourceBase* zasoby;
#endif

#endif