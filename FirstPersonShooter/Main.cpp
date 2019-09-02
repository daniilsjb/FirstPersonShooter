#include "EngineFPS.h"

int main()
{
	EngineFPS game;
	if (game.ConstructScreen(320, 240, 4, 4))
		game.Start();

	return 0;

}