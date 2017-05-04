#include <iostream>
#include "Game.h"

using namespace std;


int main(void)
{
	try
	{
		Game mg;
		mg.run();
	}
	catch(...)
	{
		std::cout << "Fehler: Das Fenster konnte nicht initialisiert werden." << std::endl;
	}

	return 1;
}