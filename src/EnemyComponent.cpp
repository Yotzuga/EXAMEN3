#include "EnemyComponent.h"

EnemyComponent::EnemyComponent(float spd)
    : Component(), speed(spd)
{
    m_Type = "EnemyComponent";
}
