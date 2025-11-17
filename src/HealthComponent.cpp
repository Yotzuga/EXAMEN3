#include "HealthComponent.h"
#include <algorithm>

HealthComponent::HealthComponent(int maxHealth)
    : Component(), hp(maxHealth), maxHp(maxHealth)
{
    m_Type = "HealthComponent";
}

void HealthComponent::takeDamage(int damage)
{
    hp = std::max(0, hp - damage);
}

void HealthComponent::heal(int amount)
{
    hp = std::min(maxHp, hp + amount);
}

bool HealthComponent::isDead() const
{
    return hp <= 0;
}
