
#ifndef RESOURCE_LOADER_HPP
#include "resources/Arimo-Regular.hpp"
#include <SFML/Graphics.hpp>
#include <stdexcept>

#define RESOURCE_LOADER_HPP

struct ResourceBase
{
	bool correct;
};

ResourceBase* LoadResources();

#if defined(EXTERNPLEASE)
extern ResourceBase* zasoby;
extern sf::Font* fona;
#else
sf::Font* fona;
ResourceBase* zasoby;
#endif

#endif