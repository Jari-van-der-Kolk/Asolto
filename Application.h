// foo.h : Include file for standard system include files,
// or project specific include files.
#ifndef APPLICATION_H
#define APPLICATION_H

#pragma once
#include <iostream>
#include <vector>
#include <raylib/raylib.h>
#include <magique/core/Game.h>
#include <magique/ecs/Scripting.h>


#define SPACING 25
#define BOARD_ARRAY_SIZE 7
#define GRID_POSITION_X 100 //both sets the same x and y pos imagine a diagonal
#define GRID_POSITION_Y 100 //both sets the same x and y pos imagine a diagonal
#define CELL_RADIUS 25 // sets the cells radius
#define PADDING 50

using namespace std;

//todo-list make a template for everything that you need to fully maximize the engine capabilities

//todo make sure that you can transition between levels

using namespace magique;

struct Position
{
    short x, y;
    vector<pair<int,int>>* connections;  // Use a vector of pointers to Vertex

    Position(short x, short y) : x(x), y(y)
    {
        connections = new vector<pair<int, int>>;
    }

};


// Entity identifiers
enum EntityID : uint16_t
{
    NORMAL_PAWN,
    OFFICER_PAWN,
    STATIC_CAMERA, // In this example the camera is static and not attached to a entity
};

enum class GameState
{
    GAME,
    PAUSE,
    GAME_OVER,
};

enum class MapID : uint8_t
{
    MENU_SCREEN,
    LEVEL,
    GAME_OVER_LEVEL,
};

struct Grid
{
    void SetSurroundingPositions(Position* pos, int i, int j, const vector<pair<int, int>>& directions);
    void GenerateVertices(const int pivotX, const  int pivotY);
    void DrawPieces();
    Position GridToBoardPosition(int i, int j);

    int gridConnections[BOARD_ARRAY_SIZE][BOARD_ARRAY_SIZE] =
    {
        {0,0,1,1,1,0,0},
        {0,0,1,2,1,0,0},
        {1,1,1,1,1,1,1},
        {1,2,1,2,1,2,1},
        {1,1,1,1,1,1,1},
        {0,0,1,2,1,0,0},
        {0,0,1,1,1,0,0}
    };
    
    int spawnLocationsGrid[BOARD_ARRAY_SIZE][BOARD_ARRAY_SIZE] =
    {
        {0,0,1,1,1,0,0},
        {0,0,1,1,1,0,0},
        {1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1},
        {1,1,2,0,0,1,1},
        {0,0,0,0,0,0,0},
        {0,0,0,0,2,0,0}
    };

    const vector<std::pair<int, int>> surroudingDirections = {
     {-1, -1}, {-1, 0}, {-1, 1},   // Top-left, top, top-right
     {0, -1},         {0, 1},      // Left,      , right
     {1, -1}, {1, 0}, {1, 1}       // Bottom-left, bottom, bottom-right
    };

    const vector<pair<int, int>> neighborDirections = {
            {-1, 0},    // Top-left, top, top-right
     {0, -1},         {0, 1},      // Left,      , right
             {1, 0}
    };


private:
    Position* connectionVertices[BOARD_ARRAY_SIZE * BOARD_ARRAY_SIZE - 16];
    Position* spawnLocationVertices[BOARD_ARRAY_SIZE * BOARD_ARRAY_SIZE - 16];
};

struct Application final : Game
{
public:
    Application() : Game("Alquerque") {}
    ~Application(){}
    void onStartup(AssetLoader& loader, GameConfig& config) override;
    void updateGame(GameState gameState) override;
    void drawGame(GameState gameState, Camera2D& camera) override;
    void onCloseEvent() override;
    

private:
    Grid* gridConnections = new Grid();
};


#endif // DEsBUG
