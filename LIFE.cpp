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
	olc::vi2d vWorldSize = { 64, 45 };
	int nTotalCells = vWorldSize.x * vWorldSize.y;
	bool *bCells = new bool [nTotalCells];

private:
	void ClearMap()
	{
		for (int i = 0; i < nTotalCells; i++)
			bCells[i] = false;
	}

private:
	bool OnUserCreate() override
	{
		ClearMap();
		tv = olc::TileTransformedView({ ScreenWidth(), ScreenHeight() }, { ScreenWidth() / vWorldSize.x, ScreenHeight() / vWorldSize.y });
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Placing cells
		if (GetMouse(0).bHeld)
		{
			olc::vi2d vClickPos = tv.ScreenToWorld(GetMousePos());
			int index = vClickPos.y * vWorldSize.x + vClickPos.x;
			bCells[index] = true;
		}
		else if (GetMouse(1).bHeld)
		{
			olc::vi2d vClickPos = tv.ScreenToWorld(GetMousePos());
			int index = vClickPos.y * vWorldSize.x + vClickPos.x;
			bCells[index] = false;
		}

		// Keyboard controls
		if (GetKey(olc::C).bPressed)
		{
			std::cout << "Clearing map...\n";
			ClearMap();
		}

//		for (int y = 0; y < vWorldSize.y; y++)
//		{
//			for (int x = 0; x < vWorldSize.x; x++)
//			{
//				// Decide the fate of each cell...
//			}
//		}

		Clear(olc::DARK_BLUE);

		for (float y = 0; y < vWorldSize.y; y++)
		{
			for (float x = 0; x < vWorldSize.x; x++)
			{
				int index = y * vWorldSize.x + x;
				if (bCells[index])
					tv.FillRect({ x, y }, { 1, 1 }, olc::WHITE);
			}
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
