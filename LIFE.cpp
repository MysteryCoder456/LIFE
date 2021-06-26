#include <iostream>
#include <algorithm>
#include <iterator>

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
    olc::vi2d vWorldSize = {64, 45};
    int nTotalCells = vWorldSize.x * vWorldSize.y;

    std::vector<bool> bCells = {};
    std::vector<bool> bTempCells = {};

    const float fFrameTime = 0.4f;
    float fFrameElapsedTime = 0.0f;

private:
    bool bPause = true;

private:
    void ClearMap()
    {
        std::cout << "Clearing map...\n";
        for (int i = 0; i < nTotalCells; i++)
            bCells[i] = false;
    }

    int CountNeighbors(olc::vi2d cellPos)
    {
        int sum = 0;

        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                int x = int(cellPos.x + j + vWorldSize.x) % vWorldSize.x;
                int y = int(cellPos.y + i + vWorldSize.y) % vWorldSize.y;

                if (cellPos == olc::vi2d(x, y))
                    continue;

                int index = y * vWorldSize.x + x;
                sum += bCells[index];
            }
        }

        return sum;
    }

private:
    bool OnUserCreate() override
    {
        // Initialize primary and secondary maps
        for (int i = 0; i < nTotalCells; i++)
        {
            bCells.push_back(false);
            bTempCells.push_back(false);
        }

        ClearMap();
        tv = olc::TileTransformedView({ScreenWidth(), ScreenHeight()}, {ScreenWidth() / vWorldSize.x, ScreenHeight() / vWorldSize.y});
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

        if (!bPause && fFrameElapsedTime >= fFrameTime)
        {
            // Clear secondary map
            for (int i = 0; i < nTotalCells; i++)
                bTempCells[i] = false;

            // Calculate the fate of each cell without modifying original map
            for (int y = 0; y < vWorldSize.y; y++)
            {
                for (int x = 0; x < vWorldSize.x; x++)
                {
                    int index = y * vWorldSize.x + x;
                    int nNeighbourCount = CountNeighbors({x, y});

                    bool bCellAlive = bCells[index];
                    if (bCellAlive)
                    {
                        if (nNeighbourCount > 1 && nNeighbourCount < 4)
                            bTempCells[index] = true;
                    }
                    else if (nNeighbourCount == 3)
                        bTempCells[index] = true;
                }
            }

            // Copy secondary map to actual map
            bCells = bTempCells;

            // Reset elapsed frame time to zero
            fFrameElapsedTime = 0.0f;
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
                    tv.FillRect({x, y}, {1, 1}, olc::WHITE);
            }
        }

        fFrameElapsedTime += fElapsedTime;
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
