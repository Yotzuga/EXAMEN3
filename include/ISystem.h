#pragma once

class World; // Forward declaration

struct ISystem
{
    virtual void update(World &world, float dt) = 0;
    virtual ~ISystem() = default;
};