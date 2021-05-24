#include <iostream>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"


class GameOfLife : public olc::PixelGameEngine
{
public:
	GameOfLife()
	{
		sAppName = "Conway's Game of Life";
	}

private:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::DARK_BLUE);
		return true;
	}
};


int main(int argc, char **argv)
{
	GameOfLife game;
	if (game.Construct(1024, 720, 1, 1))
		game.Start();

	return 0;
}
