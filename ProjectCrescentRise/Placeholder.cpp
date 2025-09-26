#include "Placeholder.h"
#include "Update.h"

void Placeholder::init()
{
	Update::append(update);
}

void Placeholder::update()
{
	num++;
	std::cout << num << std::endl;
}
