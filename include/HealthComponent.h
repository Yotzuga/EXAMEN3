#pragma once
#include "Component.h"

class HealthComponent : public Component
{
public:
    int hp;    // Puntos de vida actuales
    int maxHp; // Puntos de vida máximos

    HealthComponent(int maxHealth = 3);

    void takeDamage(int damage);
    void heal(int amount);
    bool isDead() const;
};
