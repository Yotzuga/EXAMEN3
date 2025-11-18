#include "EnemyAISystem.h"
#include "Entity.h"
#include "EnemyComponent.h"
#include "PlayerComponent.h"
#include "TransformComponent.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

EnemyAISystem::EnemyAISystem()
{
}

void EnemyAISystem::update(World &world, float dt)
{
    auto entities = world.GetEntities();

    // Buscar al jugador , Se puede mejorar buscando solo una vez fuera del bucle. pero ya se hizo el template GetComponent<>
    Entity *player = nullptr;
    TransformComponent *playerTransform = nullptr;

    for (Entity *e : entities)
    {
        auto *playerComp = e->GetComponent<PlayerComponent>();
        if (playerComp)
        {
            player = e;
            playerTransform = e->GetComponent<TransformComponent>();
            break;
        }
    }

    if (!player || !playerTransform)
        return; // No hay jugador.

    // Actualizar todos los enemigos: solo cambiar dirección si jugador está en zona de focus
    for (Entity *e : entities)
    {
        auto *enemy = e->GetComponent<EnemyComponent>();
        if (!enemy)
            continue; // No es un enemigo

        auto *EnemyTransform = e->GetComponent<TransformComponent>();
        if (!EnemyTransform)
            continue;

        // Calcular distancia al jugador
        glm::vec2 direction = playerTransform->m_Position - EnemyTransform->m_Position;
        float distance = glm::length(direction);

        // SOLO modificar velocidad si el jugador está dentro del rango de focus
        if (distance <= FOCUS_RANGE && distance > 0.1f)
        {
            // Jugador en zona de focus: perseguir al jugador
            direction = glm::normalize(direction);
            EnemyTransform->m_Velocity = direction * enemy->speed;
        }
        // Si jugador está FUERA de la zona de focus:
        // NO HACER NADA - el enemigo mantiene su velocidad actual (aleatoria inicial)
        // y seguirá moviéndose en línea recta rebotando en los bordes
    }
}
