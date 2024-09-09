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
    std::cout << BOARD_ARRAY_SIZE * BOARD_ARRAY_SIZE - 16 << std::endl;
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

Position Grid::GridToBoardPosition(int i, int j)
{
    //generate the vertex position
    short x = GRID_POSITION_X + i * SPACING;
    short y = GRID_POSITION_Y + j * SPACING;

    return {x,y};
};

void Grid::SetSurroundingPositions(Position* pos, int i, int j, const vector<pair<int, int>>& directions)
{
    for(const auto& dir : directions)
    {
        int new_i = i + dir.first;
        int new_j = j + dir.second;

        if(new_i >= 0 && new_i < BOARD_ARRAY_SIZE && new_j >= 0 && new_j < BOARD_ARRAY_SIZE)
        {
            // Ensure the neighbor is within the bounds and check gridConnections
            if(gridConnections[new_i][new_j] != 0)
            {
                pos->connections->emplace_back(new_i, new_j);
            }
        }
    }

}




void Grid::GenerateVertices(const int pivotX, const int pivotY)
{
    int index = 0;

    for(int i = 0; i < BOARD_ARRAY_SIZE; i++)
    {
        for(int j = 0; j < BOARD_ARRAY_SIZE; j++)
        {
            //check if connection type is not empty
            int connectionType = gridConnections[i][j];
            if(connectionType == 0)
                continue;

            short x = GRID_POSITION_X + i * SPACING;
            short y = GRID_POSITION_Y + j * SPACING;
            
            connectionVertices[index] = new Position(x, y);
            Position* v = connectionVertices[index];

            //generate the connections
            if(connectionType == 1)
            {
                SetSurroundingPositions(v, i, j, neighborDirections);
            }
            else if(connectionType == 2)
            {
                SetSurroundingPositions(v, i, j, surroudingDirections);
            }

            index++;
        }
    }

    //std::cout << index << std::endl;
}

void Grid::DrawPieces()
{
    int length = sizeof(connectionVertices) / sizeof(connectionVertices[0]);
    for(int i = 0; i < length; i++)
    {
        int x0 = connectionVertices[i]->x;
        int y0 = connectionVertices[i]->y;
        DrawCircle(x0, y0, CELL_RADIUS * .25f, BLACK);

        auto& cVert = connectionVertices[i]->connections;
        
        for(int j = 0; j < cVert->size(); j++)
        {
            auto pos = GridToBoardPosition(i, j);
            DrawLine(x0, y0, pos.x, pos.y, BLACK);
        }
    }

    

}




