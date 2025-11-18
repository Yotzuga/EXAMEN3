#include "DamageSystem.h"
#include "Entity.h"
#include "HealthComponent.h"
#include "Event.h"
#include <spdlog/spdlog.h>

DamageSystem::DamageSystem(bool *gameRunning, bool *gameOver)
    : m_GameRunning(gameRunning), m_GameOver(gameOver)
{
}

void DamageSystem::update(World &world, float dt)
{
    // Procesar todos los eventos DamageEvent
    std::unique_ptr<Event> event;
    while (world.poll(event))
    {
        if (event->getType() == "DamageEvent")
        {
            DamageEvent *damageEvent = static_cast<DamageEvent *>(event.get());

            spdlog::warn("Procesando DamageEvent: Entidad={}, Danio={}",
                         damageEvent->entityId, damageEvent->damage);

            // Buscar la entidad objetivo
            auto entities = world.GetEntities();
            for (Entity *e : entities)
            {
                if (e->m_Id == damageEvent->entityId)
                {
                    auto *health = e->GetComponent<HealthComponent>();
                    if (health)
                    {
                        // Reducir HP usando takeDamage
                        health->takeDamage(damageEvent->damage);

                        spdlog::info("Entidad {} recibio {} de danio. HP: {}/{}",
                                     e->m_Id, damageEvent->damage,
                                     health->hp, health->maxHp);

                        // Verificar si murio
                        if (health->isDead())
                        {
                            spdlog::error("GAME OVER Entidad {} murio", e->m_Id);

                            // Activar Game Over
                            if (m_GameOver)
                            {
                                *m_GameOver = true;
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}