#include <iostream>
#include "Game.h"

int main(void)
{
	try
	{
		Game mg;
		mg.run();
	}
	catch(const std::exception& ex)
	{
		std::cout << ex.what() << " Druecke eine Taste zum beenden." << std::endl;
		getchar();
	}

	return 1;
}