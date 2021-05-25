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
	bool bPause = true;

private:
	void ClearMap()
	{
		std::cout << "Clearing map...\n";
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
			ClearMap();
		if (GetKey(olc::P).bPressed)
			bPause = !bPause;

		if (!bPause)
		{
			// Create a temporary map
			bool *bTempCells = new bool [nTotalCells];
			for (int i = 0; i < nTotalCells; i++)
				bTempCells[i] = false;

			// Run calculate the fate of each cell without modifying original map
			for (int y = 0; y < vWorldSize.y; y++)
			{
				for (int x = 0; x < vWorldSize.x; x++)
				{
					int index = y * vWorldSize.x + x;
					int TLIndex = (y - 1) * vWorldSize.x + (x - 1);
					int TMIndex = (y - 1) * vWorldSize.x + x;
					int TRIndex = (y - 1) * vWorldSize.x + (x + 1);
					int MLIndex = y * vWorldSize.x + (x - 1);
					int MRIndex = y * vWorldSize.x + (x + 1);
					int BLIndex = (y + 1) * vWorldSize.x + (x - 1);
					int BMIndex = (y + 1) * vWorldSize.x + x;
					int BRIndex = (y + 1) * vWorldSize.x + (x + 1);

					int nNeighbourCount =
							bCells[TLIndex] +
							bCells[TMIndex] +
							bCells[TRIndex] +
							bCells[MLIndex] +
							bCells[MRIndex] +
							bCells[BLIndex] +
							bCells[BMIndex] +
							bCells[BRIndex];

					bool bCellAlive = bCells[index];
					if (bCellAlive)
					{
						if (nNeighbourCount < 2 || nNeighbourCount > 3)
							bTempCells[index] = false;
					}
					else if (nNeighbourCount == 3)
						bTempCells[index] = true;
				}
			}

			// Set actual map to temporary map
			bCells = bTempCells;
		}

		Clear(olc::DARK_BLUE);
		if (bPause)
			DrawString(0, 0, "Paused!");

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
