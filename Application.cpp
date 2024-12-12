#include "Application.h"
#include "Application.h"

#include <sys/stat.h>

#include "GLFW/glfw3.h"
//0011100
//0011100
//1111111
//1111111
//1122211
//0022200
//0022200

static Grid asoltoBoard;
static bool clicked = false;
static entt::entity previousSlot;

void SlotScript::onCreate(entt::entity self)
{
    EntityScript::onCreate(self);

}

void SlotScript::onMouseEvent(entt::entity self)
{
    // Call the parent class method
    EntityScript::onMouseEvent(self);

    auto& slot = GetComponent<SlotC>(self);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !slot.clicked)
    {
        slot.clicked = true;
        if (slot.mouseInBounds(self))
        {

            // Check if the previousSlot is valid and contains the SlotC component
            if (previousSlot != entt::null && internal::REGISTRY.valid(previousSlot) && internal::REGISTRY.all_of<SlotC>(previousSlot))
            {
                if (slot.pawnType == PawnType::NONE)
                {
                    auto& otherSlot = GetComponent<SlotC>(previousSlot);
                    if (slot.Contains(otherSlot.position) || otherSlot.Contains(slot.position))
                    {
                        slot.SetPawnType(otherSlot.pawnType);
                        otherSlot.SetPawnType(PawnType::NONE);
                    }
                }
            }
            slot.selected = !slot.selected;
            previousSlot = self;
        }
    }
    else
    {
        slot.clicked = false;
        if (previousSlot != self)
        {
            slot.selected = false;
        }
    }

}

void Application::GenerateMap(float slotRadius)
{
    Vector2 middlePoint(GetScreenWidth() / 2, GetScreenHeight() / 2);

    // Store slot entities in a 2D array to access them later

    for (int y = 0; y < BOARD_ARRAY_SIZE; y++)
    {
        for (int x = 0; x < BOARD_ARRAY_SIZE; x++)
        {
            int cell = asoltoBoard.grid[y][x];
            if (cell >= 1)
            {
                float posX = middlePoint.x + (x - BOARD_ARRAY_SIZE / 2) * slotRadius;
                float posY = middlePoint.y + (y - BOARD_ARRAY_SIZE / 2) * slotRadius;
                auto slot = CreateEntity(SLOT, posX, posY, MapID::LEVEL);

                auto& slotC = GetComponent<SlotC>(slot);
                slotC.SetPosition(make_pair(y, x));
                slotC.SetRadius(30);

                if (cell == 1 || cell == 4)
                {
                    slotC.SetPawnType(PawnType::SOLDIER);
                    if (cell == 4)
                    {
                        slotC.SetNeighbors(asoltoBoard.surroudingDirections);
                    }
                    else
                    {
                        slotC.SetNeighbors(asoltoBoard.neighborDirections);
                    }
                }
                else if (cell == 2)
                {
                    slotC.SetPawnType(PawnType::LIEUTENANT);
                }
                else if (cell == 3 || cell == 5)
                {
                    slotC.SetPawnType(PawnType::NONE);
                    if (cell == 5)
                    {
                        slotC.SetNeighbors(asoltoBoard.surroudingDirections);
                    }
                    else
                    {
                        slotC.SetNeighbors(asoltoBoard.neighborDirections);
                    }
                }
            }
        }
    }
}


void Application::onStartup(AssetLoader& loader)
{
    SetTargetFPS(144);
    SetTraceLogLevel(LOG_NONE);
    SetStaticWorldBounds({0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}); // Easy way to set up world bounds

    SetEntityScript(SLOT, new SlotScript());

    RegisterEntity(SLOT, [](entt::entity entity, EntityType type)
    {
        GiveActor(entity);
        GiveScript(entity);
        GiveComponent<SlotC>(entity);
    });


    RegisterEntity(STATIC_CAMERA, [](entt::entity entity, EntityType type) { GiveCamera(entity); });

    CreateEntity(STATIC_CAMERA, GetScreenWidth() / 2, GetScreenHeight() / 2, MapID::LEVEL);

    GenerateMap(30);
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
    ClearBackground(BLACK);
    auto& drawEntities = GetDrawEntities(); // Get the entities that need to be drawn
    for (auto& e : drawEntities)
    {
        auto& pos = GetComponent<PositionC>(e); // Get the implicit position component
        switch (pos.type)
        {
        case SLOT:
            auto& slot = GetComponent<SlotC>(e);

            DrawRectangleLines(pos.x, pos.y, slot.radius, slot.radius, WHITE);

            if (slot.selected)
            {
                DrawRectangleLines(pos.x, pos.y, slot.radius, slot.radius - 4, WHITE);
            }

            Vector2 pawnOffset = { pos.x + (slot.radius * .5f), pos.y + (slot.radius * .5f) };
            if (slot.pawnType == PawnType::SOLDIER)
            {
                DrawCircle(pawnOffset.x, pawnOffset.y, slot.radius * .25f, WHITE);
            }
            if (slot.pawnType == PawnType::LIEUTENANT)
            {
                DrawCircle(pawnOffset.x, pawnOffset.y, slot.radius * .25f, BLUE);
            }

            break;
        }
    }

    EndDrawing();

};
void Application::onCloseEvent()
{

    shutDown();
}



