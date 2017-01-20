
#include <ExtendedEngine/ApplicationManager.h>

#include "GameState.h"

#include <string>


int main()
{
	extspehs::ApplicationManager::init("FGJ17", new GameState);
	extspehs::ApplicationManager::uninit();
	return 0;
}