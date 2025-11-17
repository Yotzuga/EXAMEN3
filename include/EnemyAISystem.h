#pragma once
#include "ISystem.h"
#include "World.h"

class EnemyAISystem : public ISystem
{
private:
    const float FOCUS_RANGE = 150.0f; // Rango de persecución al jugador

public:
    EnemyAISystem();
    void update(World &world, float dt) override;
};
