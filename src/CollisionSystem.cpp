#include "CollisionSystem.h"
#include "Entity.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "Event.h"
#include <spdlog/spdlog.h>

CollisionSystem::CollisionSystem()
{
}

void CollisionSystem::SetWorldDimensions(float width, float height)
{
    m_WorldWidth = width;
    m_WorldHeight = height;
}

// AABB del Profesor
bool CollisionSystem::checkAABBCollision(const glm::vec2 &pos1, const glm::vec2 &size1,
                                         const glm::vec2 &pos2, const glm::vec2 &size2)
{
    // Calcular los bordes de ambos rectángulos
    // Usar el centro del rectángulo para la comparacion
    const float r1Left = pos1.x;
    const float r1Right = pos1.x + size1.x;
    const float r1Top = pos1.y;
    const float r1Bottom = pos1.y + size1.y;

    const float r2Left = pos2.x;
    const float r2Right = pos2.x + size2.x;
    const float r2Top = pos2.y;
    const float r2Bottom = pos2.y + size2.y;

    // Verificar si NO hay colision, luego negar
    return !(r1Left > r2Right || r1Right < r2Left || r1Bottom < r2Top || r1Top > r2Bottom);
}

// refactorizado del EXAMEN 2
void CollisionSystem::checkWorldBoundaries(const std::vector<Entity *> &entities)
{
    // Verificar colisiones de entidades con los límites del mundo (rebotes en bordes)
    for (Entity *e : entities)
    {
        auto *t = e->GetComponent<TransformComponent>();
        auto *c = e->GetComponent<ColliderComponent>();

        if (!t || !c)
            continue;

        // Verificar colisión con los límites del mundo (sin logging para evitar spam)
        if (t->m_Position.x < 0.f)
        {
            t->m_Position.x = 0.f;
            t->m_Velocity.x *= -1.f;
        }
        else if (t->m_Position.x + c->m_Bounds.x > m_WorldWidth)
        {
            t->m_Position.x = m_WorldWidth - c->m_Bounds.x;
            t->m_Velocity.x *= -1.f;
        }

        if (t->m_Position.y < 0.f)
        {
            t->m_Position.y = 0.f;
            t->m_Velocity.y *= -1.f;
        }
        else if (t->m_Position.y + c->m_Bounds.y > m_WorldHeight)
        {
            t->m_Position.y = m_WorldHeight - c->m_Bounds.y;
            t->m_Velocity.y *= -1.f;
        }
    }
}

void CollisionSystem::update(World &world, float dt)
{
    auto entities = world.GetEntities();

    // Actualizar cooldowns existentes
    for (auto it = m_CollisionCooldowns.begin(); it != m_CollisionCooldowns.end();)
    {
        it->second -= dt;
        if (it->second <= 0.0f)
        {
            it = m_CollisionCooldowns.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // 1. Verificar colisiones del jugador con enemigos
    Entity *player = nullptr;
    for (Entity *e : entities)
    {
        auto *playerComp = e->GetComponent<PlayerComponent>();
        if (playerComp)
        {
            player = e;
            break;
        }
    }

    if (player)
    {
        auto *playerTransform = player->GetComponent<TransformComponent>();
        auto *playerCollider = player->GetComponent<ColliderComponent>();

        if (playerTransform && playerCollider)
        {
            // Verificar colisiones del jugador con todos los enemigos
            for (Entity *e : entities)
            {
                auto *enemyComp = e->GetComponent<EnemyComponent>();
                if (!enemyComp)
                    continue; // No es un enemigo

                auto *enemyTransform = e->GetComponent<TransformComponent>();
                auto *enemyCollider = e->GetComponent<ColliderComponent>();

                if (!enemyTransform || !enemyCollider)
                    continue;

                // Verificar colision AABB
                if (checkAABBCollision(playerTransform->m_Position, playerCollider->m_Bounds,
                                       enemyTransform->m_Position, enemyCollider->m_Bounds))
                {
                    // Crear clave única para este par
                    std::string collisionKey = player->m_Id + ":" + e->m_Id;

                    // Solo procesar si no está en cooldown
                    if (m_CollisionCooldowns.find(collisionKey) == m_CollisionCooldowns.end())
                    {
                        spdlog::warn("¡Colision detectada! Jugador {} <-> Enemigo {} en ({:.1f},{:.1f})",
                                     player->m_Id, e->m_Id, playerTransform->m_Position.x, playerTransform->m_Position.y);

                        // Emitir evento DamageEvent (1 punto de daño para morir en 3 toques)
                        auto damageEvent = std::make_unique<DamageEvent>(player->m_Id, 1);
                        world.emit(std::move(damageEvent));

                        // Empujar al jugador hacia atrás para evitar colisiones continuas
                        glm::vec2 pushDirection = glm::normalize(playerTransform->m_Position - enemyTransform->m_Position);
                        playerTransform->m_Position += pushDirection * 10.0f;

                        // Agregar cooldown
                        m_CollisionCooldowns[collisionKey] = m_CooldownTime;
                    }
                }
            }
        }
    }

    // 2. Verificar colisiones entre enemigos
    for (size_t i = 0; i < entities.size(); ++i)
    {
        Entity *entityA = entities[i];
        auto *enemyA = entityA->GetComponent<EnemyComponent>();

        if (!enemyA)
            continue; // No es un enemigo

        auto *transformA = entityA->GetComponent<TransformComponent>();
        auto *colliderA = entityA->GetComponent<ColliderComponent>();

        if (!transformA || !colliderA)
            continue;

        // Verificar contra todos los enemigos posteriores (evitar duplicados)
        for (size_t j = i + 1; j < entities.size(); ++j)
        {
            Entity *entityB = entities[j];
            auto *enemyB = entityB->GetComponent<EnemyComponent>();

            if (!enemyB)
                continue; // No es un enemigo

            auto *transformB = entityB->GetComponent<TransformComponent>();
            auto *colliderB = entityB->GetComponent<ColliderComponent>();

            if (!transformB || !colliderB)
                continue;

            // Verificar colisión AABB entre enemigos
            if (checkAABBCollision(transformA->m_Position, colliderA->m_Bounds,
                                   transformB->m_Position, colliderB->m_Bounds))
            {
                // Crear clave única para este par (ordenada alfabéticamente)
                std::string collisionKey = (entityA->m_Id < entityB->m_Id)
                                               ? entityA->m_Id + ":" + entityB->m_Id
                                               : entityB->m_Id + ":" + entityA->m_Id;

                // Solo procesar si no está en cooldown
                if (m_CollisionCooldowns.find(collisionKey) == m_CollisionCooldowns.end())
                {
                    // Calcular dirección de empuje para separarlos
                    glm::vec2 centerA = transformA->m_Position + (colliderA->m_Bounds * 0.5f);
                    glm::vec2 centerB = transformB->m_Position + (colliderB->m_Bounds * 0.5f);
                    glm::vec2 pushDirection = glm::normalize(centerA - centerB);

                    // Empujar ambos enemigos en direcciones opuestas
                    float pushStrength = 10.0f;
                    transformA->m_Position += pushDirection * pushStrength;
                    transformB->m_Position -= pushDirection * pushStrength;

                    // Invertir velocidades de ambos enemigos
                    transformA->m_Velocity *= -1.0f;
                    transformB->m_Velocity *= -1.0f;

                    // Agregar cooldown
                    m_CollisionCooldowns[collisionKey] = m_CooldownTime;
                }
            }
        }
    }

    // 3. Verificar colisiones con los límites del mundo (rebotes en bordes)
    checkWorldBoundaries(entities);
}
