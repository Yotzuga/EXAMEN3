#pragma once
#include "ISystem.h"
#include "World.h"

class MovementSystem : public ISystem
{
public:
    MovementSystem();

    void update(World &world, float dt) override;
};
