#include "Event.h"

// DamageEvent implementation
DamageEvent::DamageEvent(const std::string &id, int dmg)
    : entityId(id), damage(dmg)
{
}

std::string DamageEvent::getType() const
{
    return "DamageEvent";
}

// SpawnEvent implementation
SpawnEvent::SpawnEvent(const std::string &type, float posX, float posY)
    : entityType(type), x(posX), y(posY)
{
}

std::string SpawnEvent::getType() const
{
    return "SpawnEvent";
}
