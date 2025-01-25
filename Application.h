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
#include <ankerl/unordered_dense.h>


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
    int defeultGrid[BOARD_ARRAY_SIZE][BOARD_ARRAY_SIZE] =
    {
        {0,0,1,1,1,0,0},
        {0,0,1,4,1,0,0},
        {1,1,1,1,1,1,1},
        {1,4,1,1,1,4,1},
        {1,1,2,3,3,1,1},
        {0,0,3,5,3,0,0},
        {0,0,3,3,2,0,0}
    };

    const vector<pair<int, int>> surroudingDirections = {
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

enum class PawnType : uint8_t
{
    NONE = 0,
    SOLDIER = 1,
    LIEUTENANT = 2
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
    //config
    pair<short, short> position;
    float radius;
    PawnType pawnType;
    multimap<short, short>* connections = new multimap<short, short>;

    //state
    bool selected;
    bool clicked = false;


    void SetPosition(pair<short, short> position)
    {
        this->position = position;
    }

    void SetPawnType(PawnType pawnType)
    {
        this->pawnType = pawnType;
    }

    void SetRadius(float radius)
    {
        this->radius = radius;
    }

    PawnType GetPawnType() const
    {
        return this->pawnType;
    }

    bool MouseInBounds(entt::entity& entity)
    {
        auto& pos = GetComponent<PositionC>(entity);
        // Define the AABB rectangle around the circle
        Rectangle aabb = { pos.x, pos.y, radius, radius };

        // Get the mouse position
        Vector2 mousePos = GetMousePosition();

        // Check if the mouse is within the rectangle
        return CheckCollisionPointRec(mousePos, aabb);
    }

    bool Contains(pair<short, short>& value)
    {
        for (auto& pair : *connections)
        {
            if (pair.first == value.first && pair.second == value.second)
                return true;
        }
        return false;
    }

    void SetNeighbors(const vector<std::pair<int, int>>& directions)
    {
        for(const auto& dir : directions)
        {
            int new_y = position.first + dir.first;
            int new_x = position.second + dir.second;

            if(new_y >= 0 && new_y < BOARD_ARRAY_SIZE && new_x >= 0 && new_x < BOARD_ARRAY_SIZE)
            {
                pair<int, int> connectedIndex = make_pair(new_y, new_x);
                connections->emplace(connectedIndex);
            }
        }
    }
};


//scripts
struct SlotScript final : EntityScript
{
    void onMouseEvent(entt::entity self) override;
};

struct Application final : Game
{
public:

    Application() : Game("Asolto") {}
    ~Application(){}
    void onStartup(AssetLoader& loader) override;
    void updateGame(GameState gameState) override;
    void drawGame(GameState gameState, Camera2D& camera) override;
    void onCloseEvent() override;
    

private:
};




#endif // DEsBUG
