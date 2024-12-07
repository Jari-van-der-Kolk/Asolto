// foo.h : Include file for standard system include files,
// or project specific include files.
#ifndef APPLICATION_H
#define APPLICATION_H

#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <utility>  // for std::make_pair
#include <raylib/raylib.h>
#include <magique/magique.hpp>


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

class Grid
{
public:
    int grid[BOARD_ARRAY_SIZE][BOARD_ARRAY_SIZE] =
    {
        {0,0,1,1,1,0,0},
        {0,0,1,1,1,0,0},
        {1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1},
        {1,1,2,3,3,1,1},
        {0,0,3,3,3,0,0},
        {0,0,3,3,2,0,0}
    };



    const vector<std::pair<int, int>> surroudingDirections = {
        {-1, -1}, {-1, 0}, {-1, 1},   // Top-left, top, top-right
        {0, -1},         {0, 1},      // Left,      l, right
        {1, -1}, {1, 0}, {1, 1}       // Bottom-left, bottom, bottom-right
    };

    const vector<pair<int, int>> neighborDirections = {
        {-1, 0},    // Top-left, top, top-right
 {0, -1},         {0, 1},      // Left,      , right
         {1, 0}
    };
};


// Entity identifiers
enum EntityType : uint16_t
{
    SLOT,
    STATIC_CAMERA, // In this example the camera is static and not attached to a entity
};

enum class MapID : uint8_t
{
    LEVEL,
    GAME_OVER_LEVEL,
};

enum class GameState
{
    GAME,
    PAUSE,
    GAME_OVER,
};

//components

struct SlotC
{
    float radius = 10;
    bool soldier;
    bool lieutenant;
    bool selected;
    bool clicked = false;

    void SetRadius(float radius)
    {
        this->radius = radius;
    }


    bool mouseInBounds(entt::entity entity)
    {
        auto& pos = GetComponent<PositionC>(entity);
        // Define the AABB rectangle around the circle
        Rectangle aabb = { pos.x - radius, pos.y - radius, radius * 2, radius * 2 };

        // Get the mouse position
        Vector2 mousePos = GetMousePosition();

        // Check if the mouse is within the rectangle
        return CheckCollisionPointRec(mousePos, aabb);
    }
};


//scripts
struct SlotScript final : EntityScript
{
    void onCreate(entt::entity self) override;
    void onMouseEvent(entt::entity self) override;
};

struct Application final : Game
{
public:

    void GenerateMap(float slotRadius);

    Application() : Game("Asolto") {}
    ~Application(){}
    void onStartup(AssetLoader& loader) override;
    void updateGame(GameState gameState) override;
    void drawGame(GameState gameState, Camera2D& camera) override;
    void onCloseEvent() override;
    

private:
};


#endif // DEsBUG
