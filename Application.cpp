#include "Application.h"
#include "Application.h"

#include <ankerl/unordered_dense.h>
#include <sys/stat.h>

#include "GLFW/glfw3.h"


static Grid asoltoBoard;
static bool clicked = false;
static entt::entity previousSlot;
static PawnType turnType = PawnType::LIEUTENANT;
static bool turn = false;
static ankerl::unordered_dense::map<pair<short,short>, SlotC*>  slotPositions;



void SlotScript::onMouseEvent(entt::entity self)
{
    // Call the parent class method
    EntityScript::onMouseEvent(self);

    auto& selectedSlot = GetComponent<SlotC>(self);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !selectedSlot.clicked)
    {
        selectedSlot.clicked = true;
        if (selectedSlot.mouseInBounds(self))
        {
            if (previousSlot != entt::null && internal::REGISTRY.valid(previousSlot) && internal::REGISTRY.all_of<SlotC>(previousSlot))
            {
                auto& previousSlotComponent = GetComponent<SlotC>(previousSlot);

                const bool containsConnection = selectedSlot.Contains(previousSlotComponent.position) || previousSlotComponent.Contains(selectedSlot.position);
                const bool isSlotEmpty = selectedSlot.pawnType == PawnType::NONE;
                const bool turnTypeCheck = previousSlotComponent.pawnType == turnType;

                if (containsConnection && isSlotEmpty && turnTypeCheck)
                {
                        turn = !turn;
                        turnType = turn ? PawnType::SOLDIER : PawnType::LIEUTENANT;

                        selectedSlot.SetPawnType(previousSlotComponent.pawnType);
                        previousSlotComponent.SetPawnType(PawnType::NONE);
                }
                else if (turnTypeCheck && turnType == PawnType::LIEUTENANT)
                {
                    const int x1 = selectedSlot.position.second - previousSlotComponent.position.second + selectedSlot.position.second;
                    const int y1 = selectedSlot.position.first - previousSlotComponent.position.first + selectedSlot.position.first;

                    const pair<short,short> id = make_pair(y1,x1);

                    if (!slotPositions.contains(id))
                    {
                        goto skip;
                    }

                    SlotC* foundSlot = slotPositions.find(id)->second;

                    if (foundSlot->pawnType != PawnType::NONE)
                    {
                        goto skip;
                    }

                    const bool selectedAndPreviousConnections = selectedSlot.Contains(previousSlotComponent.position) || previousSlotComponent.Contains(selectedSlot.position);
                    const bool foundAndSelectedConnections = selectedSlot.Contains(foundSlot->position) || foundSlot->Contains(selectedSlot.position);

                    if (selectedAndPreviousConnections && foundAndSelectedConnections)
                    {
                        foundSlot->SetPawnType(previousSlotComponent.pawnType);
                        previousSlotComponent.SetPawnType(PawnType::NONE);
                        selectedSlot.SetPawnType(PawnType::NONE);

                        turn = !turn;
                        turnType = turn ? PawnType::SOLDIER : PawnType::LIEUTENANT;
                    }

                }
            }

            skip:
            selectedSlot.selected = !selectedSlot.selected;
            previousSlot = self;
            cout << static_cast<int>(turnType) << endl;
        }
    }
    else
    {
        selectedSlot.clicked = false;
        if (previousSlot != self)
        {
            selectedSlot.selected = false;
        }
    }
}

void Application::GenerateMap(float slotRadius)
{
    Vector2 middlePoint(GetScreenWidth() / 2, GetScreenHeight() / 2);

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

                slotPositions[make_pair(y,x)] = &slotC;

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



