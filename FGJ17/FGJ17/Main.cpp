
#include <ExtendedEngine/ApplicationManager.h>

#include "GameState.h"

#include <string>


int main()
{
	extspehs::ApplicationManager::init("Rotator-Män 2", new GameState);
	extspehs::ApplicationManager::uninit();
	return 0;
}
