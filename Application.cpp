#include "Application.h"
#include "Application.h"
#include "GLFW/glfw3.h"
//0011100
//0011100
//1111111
//1111111
//1122211
//0022200
//0022200

static Grid asoltoBoard;

void SlotScript::onCreate(entt::entity self)
{
    EntityScript::onCreate(self);

}

void SlotScript::onMouseEvent(entt::entity self)
{
    EntityScript::onMouseEvent(self);

    auto& slotc = GetComponent<SlotC>(self);
    if (slotc.clicked == false && IsMouseButtonDown(0) && slotc.mouseInBounds(self))
    {
        slotc.clicked = true;
        slotc.selected = true;
    }
    if ( IsMouseButtonUp(0))
    {
        slotc.clicked = false;
        slotc.selected = false;
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
                    slotC.soldier = true;
                }
                if (cell == 2)
                {
                    slotC.lieutenant = true;
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


            if (slot.soldier)
            {
                DrawCircle(pos.x, pos.y, slot.radius * .5f, WHITE);
            }

            if (slot.lieutenant)
            {
                DrawCircle(pos.x, pos.y, slot.radius * .5f, BLUE);
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



