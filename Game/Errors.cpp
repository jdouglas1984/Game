#include "Errors.h"
#include "include\SDL2\SDL.h"
//Helper function for error checking
void FatalError(std::string errorMsg)
{
	std::cerr << errorMsg << std::endl;
	std::cerr << "Press any key to quit..." << std::endl;
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
	exit(84);
}