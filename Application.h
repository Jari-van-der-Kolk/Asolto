// foo.h : Include file for standard system include files,
// or project specific include files.
#ifndef APPLICATION_H
#define APPLICATION_H

#pragma once
#include <iostream>
#include <raylib/raylib.h>
#include <magique/core/Game.h>
#include <magique/ecs/Scripting.h>


#define SPACING 3
#define GRID_SIZE 25
#define CELL_SIZE 80
#define PADDING 50


//todo-list make a template for everything that you need to fully maximize the engine capabilities

//todo make sure that you can transition between levels

using namespace magique;

typedef struct
{
    int x, y;
} Vertex;

// Entity identifiers
enum EntityID : uint16_t
{
    PLAYER,
    BULLET,
    ROCK,
    HOUSE,
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
    LEVEL_1,
    GAME_OVER_LEVEL,
};

enum PawnType
{
    NORMAL_PAWN,
    OFFICER
};

struct Grid
{

    void GenerateVertices(const int pivotX, const  int pivotY);
    void DrawPieces();

    int gridConnections[GRID_SIZE][GRID_SIZE] =
    {
        {0,0,1,1,1,0,0},
        {0,0,1,2,1,0,0},
        {1,1,1,1,1,1,1},
        {1,2,1,2,1,2,1},
        {1,1,1,1,1,1,1},
        {0,0,1,2,1,0,0},
        {0,0,1,1,1,0,0}
    };
    
    int spawnLocationsGrid[GRID_SIZE][GRID_SIZE] =
    {
        {0,0,1,1,1,0,0},
        {0,0,1,1,1,0,0},
        {1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1},
        {1,1,2,0,0,1,1},
        {0,0,0,0,0,0,0},
        {0,0,0,0,2,0,0}
    };

    short spawnLocationsGrid[GRID_SIZE][GRID_SIZE] =
    {
        {0,0,1,1,1,0,0},
        {0,0,1,1,1,0,0},
        {1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1},
        {1,1,2,0,0,1,1},
        {0,0,0,0,0,0,0},
        {0,0,0,0,2,0,0}
    };



private:
    Vertex connectionVertices[GRID_SIZE * GRID_SIZE - 16];
    Vertex spawnLocationVertices[GRID_SIZE * GRID_SIZE - 16];
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

struct OfficerPawn
{

};

struct NormalPawn
{

};





#endif // DEsBUG
