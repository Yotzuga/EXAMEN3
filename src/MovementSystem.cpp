#include "MovementSystem.h"
#include "Entity.h"
#include "TransformComponent.h"

MovementSystem::MovementSystem()
{
}

void MovementSystem::update(World &world, float dt)
{
    auto entities = world.GetEntities();

    for (Entity *e : entities)
    {
        auto *transform = e->GetComponent<TransformComponent>();

        if (!transform)
            continue;

        // Actualizar posición: position += velocity * dt
        transform->m_Position += transform->m_Velocity * dt;
    }
}
