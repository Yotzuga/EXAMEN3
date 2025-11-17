#pragma once
#include "Component.h"

class EnemyComponent : public Component
{
public:
    float speed; // Velocidad del enemigo

    EnemyComponent(float spd = 50.0f);
};
