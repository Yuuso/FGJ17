
#include <ExtendedEngine/ApplicationManager.h>

#include "GameState.h"

#include <string>


int main()
{
	extspehs::ApplicationManager::init("Rotator-M�n 2", new GameState);
	extspehs::ApplicationManager::uninit();
	return 0;
}
