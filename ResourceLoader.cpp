#include "ResourceLoader.hpp"

ResourceBase* LoadResources()
{
	ResourceBase* neter = new ResourceBase;
	neter->except_text="All correct.";
	neter->correct=true;
	return neter;
}