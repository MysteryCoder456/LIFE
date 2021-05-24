#include <iostream>
#include <algorithm>

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
	olc::TileTransformedView tv;
	std::vector<olc::vi2d> vCells;

private:
	bool OnUserCreate() override
	{
		tv = olc::TileTransformedView({ ScreenWidth(), ScreenHeight() }, { 16, 16 });
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Handle pan and zoom
		if (GetMouse(2).bPressed) tv.StartPan(GetMousePos());
		if (GetMouse(2).bHeld) tv.UpdatePan(GetMousePos());
		if (GetMouse(2).bReleased) tv.EndPan(GetMousePos());
		if (GetMouseWheel() > 0) tv.ZoomAtScreenPos(1.5f, GetMousePos());
		if (GetMouseWheel() < 0) tv.ZoomAtScreenPos(0.6f, GetMousePos());

		// Placing cells
		if (GetMouse(0).bHeld)
		{
			olc::vi2d vClickPos = tv.ScreenToWorld(GetMousePos());

			if (std::find(vCells.begin(), vCells.end(), vClickPos) == vCells.end())
				vCells.push_back(vClickPos);
		}
		else if (GetMouse(1).bHeld)
		{
			olc::vi2d vClickPos = tv.ScreenToWorld(GetMousePos());

			if (std::find(vCells.begin(), vCells.end(), vClickPos) != vCells.end())
				std::remove(vCells.begin(), vCells.end(), vClickPos);
		}

		for (int i = 0; i < vCells.size(); i++)
		{
			// Decide the fate of each cell...
		}

		Clear(olc::DARK_BLUE);

		for (olc::vi2d& cell : vCells)
		{
			tv.FillRect(cell, { 1, 1 }, olc::WHITE);
		}

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
