#include "Application.h"
#include "Application.h"

static Grid asoltoBoard;
static bool turn = false;
static PawnType turnType = PawnType::LIEUTENANT;
static bool clicked = false;
static float radius = 40;
static entt::entity previousEntity;
static ankerl::unordered_dense::map<pair<short,short>, SlotC*>  slotPositions;


void GenerateMap(float slotRadius)
{
    Vector2 middlePoint(GetScreenWidth() / 2, GetScreenHeight() / 2);

    for (int y = 0; y < BOARD_ARRAY_SIZE; y++)
    {
        for (int x = 0; x < BOARD_ARRAY_SIZE; x++)
        {
            int cell = asoltoBoard.defeultGrid[y][x];
            if (cell >= 1)
            {
                float posX = middlePoint.x + (x - BOARD_ARRAY_SIZE / 2) * slotRadius;
                float posY = middlePoint.y + (y - BOARD_ARRAY_SIZE / 2) * slotRadius;
                auto slot = CreateEntity(SLOT, posX, posY, MapID::LEVEL);

                auto& slotC = GetComponent<SlotC>(slot);
                slotC.SetPosition(make_pair(y, x));
                slotC.SetRadius(slotRadius);

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


void Draw()
{
    auto& drawEntities = GetDrawEntities(); // Get the entities that need to be drawn
    for (auto& e : drawEntities)
    {
        auto& pos = GetComponent<PositionC>(e); // Get the implicit position component

        switch (pos.type)
        {
        case SLOT:

            auto& slot = GetComponent<SlotC>(e);

            DrawRectangleLines(pos.x, pos.y, radius, radius, WHITE);

            if (slot.selected)
            {
                float reduction = 4;
                DrawRectangleLines(pos.x + reduction, pos.y + reduction , radius - reduction * 2, radius - reduction * 2, WHITE);
            }

            Vector2 pawnOffset = { pos.x + (radius * .5f), pos.y + (radius * .5f) };
            if (slot.pawnType == PawnType::SOLDIER)
            {
                DrawCircle(pawnOffset.x, pawnOffset.y, radius * .25f, WHITE);
            }
            if (slot.pawnType == PawnType::LIEUTENANT)
            {
                DrawCircle(pawnOffset.x, pawnOffset.y, radius * .25f, BLUE);
            }

            break;
        }
    }

}

void SwitchTurn()
{
    turn = !turn;
    turnType = turn ? PawnType::SOLDIER : PawnType::LIEUTENANT;
}

void MoveToEmptySlot(SlotC& nextSlot, SlotC& previousSlot)
{
    nextSlot.SetPawnType(previousSlot.pawnType);
    previousSlot.SetPawnType(PawnType::NONE);
}

void TakeSoldier(SlotC& nextSlot, SlotC& soldierSlot,SlotC& previousSlot)
{
    soldierSlot.SetPawnType(previousSlot.pawnType);
    previousSlot.SetPawnType(PawnType::NONE);
    nextSlot.SetPawnType(PawnType::NONE);
}



void SlotScript::onMouseEvent(entt::entity self)
{
    // Call the parent class method
    EntityScript::onMouseEvent(self);

    auto& nextSlot = GetComponent<SlotC>(self);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !nextSlot.clicked)
    {
        nextSlot.clicked = true;
        if (nextSlot.MouseInBounds(self))
        {
            if (previousEntity != entt::null && internal::REGISTRY.valid(previousEntity) && internal::REGISTRY.all_of<SlotC>(previousEntity))
            {
                auto& previousSlot = GetComponent<SlotC>(previousEntity);

                const bool containsConnection = nextSlot.Contains(previousSlot.position) || previousSlot.Contains(nextSlot.position);
                const bool isSlotEmpty = nextSlot.pawnType == PawnType::NONE;
                const bool turnTypeCheck = previousSlot.pawnType == turnType;

                if (containsConnection && isSlotEmpty && turnTypeCheck)
                {
                        SwitchTurn();
                        MoveToEmptySlot(nextSlot, previousSlot);

                }
                else if (turnTypeCheck && turnType == PawnType::LIEUTENANT)
                {
                    const int x1 = nextSlot.position.second - previousSlot.position.second + nextSlot.position.second;
                    const int y1 = nextSlot.position.first - previousSlot.position.first + nextSlot.position.first;

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

                    const bool selectedAndPreviousConnections = nextSlot.Contains(previousSlot.position) || previousSlot.Contains(nextSlot.position);
                    const bool foundAndSelectedConnections = nextSlot.Contains(foundSlot->position) || foundSlot->Contains(nextSlot.position);

                    if (selectedAndPreviousConnections && foundAndSelectedConnections)
                    {
                        SwitchTurn();
                        TakeSoldier(nextSlot, *foundSlot, previousSlot);
                    }

                }
            }

            skip:
            nextSlot.selected = !nextSlot.selected;
            previousEntity = self;
            cout << static_cast<int>(turnType) << endl;
        }
    }
    else
    {
        nextSlot.clicked = false;
        if (previousEntity != self)
        {
            nextSlot.selected = false;
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

    GenerateMap(radius);
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

    Draw();

    EndDrawing();

};
void Application::onCloseEvent()
{
    shutDown();
}



