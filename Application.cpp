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
            auto& originSlot = GetComponent<SlotC>(previousSlot);
            slot.selected = !slot.selected;
            if (slot.selected && slot.pawnType == PawnType::NONE)
            {
                MoveOccupier(slot, );
            }
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
    Vector2 middlePoint (GetScreenWidth() / 2, GetScreenHeight() / 2);
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

                slotC.SetRadius(30);

                if (cell == 1)
                {
                    slotC.SetPawnType(PawnType::SOLDIER);
                }
                else if (cell == 2)
                {
                    slotC.SetPawnType(PawnType::LIEUTENANT);
                }
                else
                {
                    slotC.SetPawnType(PawnType::NONE);
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



