#include "Application.h"

//0011100
//0011100
//1111111
//1111111
//1122211
//0022200
//0022200

void Application::onStartup(AssetLoader& loader, GameConfig& config)
{
    SetTargetFPS(144);
    
    gridConnections->GenerateVertices(PADDING, PADDING);
};

void Application::updateGame(GameState gameState)
{
    if(IsKeyDown(KEY_ESCAPE))
    {
        shutDown();
    }

};

void Application::drawGame(GameState gameState, Camera2D& camera)
{
    BeginDrawing();
    ClearBackground(WHITE);
    gridConnections->DrawPieces();


    EndDrawing();

};
void Application::onCloseEvent()
{
   
    shutDown();
}

void Grid::GenerateVertices(const int pivotX, const  int pivotY)
{
    int index = 0;
    for(int i = 0; i < GRID_SIZE; i++)
    {
        for(int j = 0; j < GRID_SIZE; j++)
        {
            if(gridConnections[i][j] == 0)
                continue;

            int x = pivotX + i * (GRID_SIZE + SPACING);
            int y = pivotY + j * (GRID_SIZE + SPACING);
            

            if(gridConnections[i][j] == 1 || gridConnections[i][j] == 2)
            {
                connectionVertices[index].x = x;
                connectionVertices[index].y = y;
                index++;
                continue;
            }

            if(gridConnections[i][j] == 2)
            {
                connectionVertices[index].x = x;
                connectionVertices[index].y = y;
            }
        }
    }

    std::cout << index << std::endl;
}

void Grid::DrawPieces()
{
    int length = sizeof(connectionVertices) / sizeof(connectionVertices[0]);
    for(int i = 0; i < length; i++)
    {
        DrawCircle(connectionVertices[i].x, connectionVertices[i].y, GRID_SIZE * .25f, BLACK);
    }
}




