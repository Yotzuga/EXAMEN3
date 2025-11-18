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

    // Verificar si NO hay colision
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

        // Verificar borde IZQUIERDO
        if (t->m_Position.x < 0.f)
        {
            t->m_Position.x = 0.f;
            t->m_Velocity.x *= -1.f;
        } // Verificar borde DERECHO
        else if (t->m_Position.x + c->m_Bounds.x > m_WorldWidth)
        {
            t->m_Position.x = m_WorldWidth - c->m_Bounds.x;
            t->m_Velocity.x *= -1.f;
        }
        // Similar para bordes SUPERIOR e INFERIOR
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

    // ========================================
    // SISTEMA DE COOLDOWN - ACTUALIZACIÓN
    // ========================================
    // Propósito: Reducir el tiempo de cooldown de cada colisión detectada
    // Esto evita que el mismo par de entidades genere múltiples eventos en frames consecutivos
    //
    // Flujo:
    // - Cada colisión establece un cooldown de 0.5 segundos
    // - Cada frame resta deltaTime (0.016s a 60 FPS)
    // - Cuando llega a 0.0f, se elimina del mapa

    auto it = m_CollisionCooldowns.begin();
    while (it != m_CollisionCooldowns.end()) // ← Recorre TODOS los pares
    {
        it->second -= dt; // Reduce tiempo a cada par

        if (it->second <= 0.0f)
        {
            it = m_CollisionCooldowns.erase(it); // Elimina PAR expirado
        }
        else
        {
            ++it; // Siguiente PAR
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
                    // ========================================
                    // VERIFICACIÓN DE COOLDOWN - JUGADOR
                    // ========================================
                    // Crear clave única para este par (formato: "ID_JUGADOR:ID_ENEMIGO")
                    // Ejemplo: "E0:E1" (Entidad 0 es jugador, Entidad 1 es enemigo)
                    std::string collisionKey = player->m_Id + ":" + e->m_Id;

                    // Verificar si esta combinación está EN COOLDOWN:
                    // - find() devuelve end() si NO existe en el mapa → NO hay cooldown activo → PROCESAR
                    // - find() devuelve iterador si EXISTE en el mapa → HAY cooldown activo → IGNORAR
                    if (m_CollisionCooldowns.find(collisionKey) == m_CollisionCooldowns.end())
                    {
                        // COOLDOWN NO ACTIVO - PROCESAR COLISIÓN

                        spdlog::warn("Colision Jugador {} <-> Enemigo {} en ({:.1f},{:.1f})",
                                     player->m_Id, e->m_Id, playerTransform->m_Position.x, playerTransform->m_Position.y);

                        // El DamageSystem procesará este evento en el siguiente ciclo
                        auto damageEvent = std::make_unique<DamageEvent>(player->m_Id, 1);
                        world.emit(std::move(damageEvent));

                        // Sistema de Empuje Jugador (Push)
                        // Calcula un vector desde el enemigo hacia el jugador y lo normaliza
                        // Luego desplaza al jugador 10 píxeles en esa dirección (hacia afuera)
                        glm::vec2 pushDirection = glm::normalize(playerTransform->m_Position - enemyTransform->m_Position);
                        playerTransform->m_Position += pushDirection * 10.0f;

                        // ESTABLECER COOLDOWN de 0.5 segundos
                        // Guarda la clave en el mapa con valor 0.5
                        // El loop de actualización de cooldowns lo reducirá cada frame
                        // Cuando llegue a 0.0f, se eliminará del mapa automáticamente
                        m_CollisionCooldowns[collisionKey] = m_CooldownTime;
                    }
                    //  Si está en cooldown: Se ignora esta colisión (no procesar evento)
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
                // ========================================
                // VERIFICACIÓN DE COOLDOWN - ENEMIGOS
                // ========================================
                // Crear clave única para este par (ordenada alfabéticamente para evitar duplicados)
                // Ejemplo: "E1:E2" o "E2:E1" → Se normaliza a "E1:E2" (menor ID primero)
                // Esto evita crear dos cooldowns diferentes para la misma colisión
                std::string collisionKey = (entityA->m_Id < entityB->m_Id)
                                               ? entityA->m_Id + ":" + entityB->m_Id
                                               : entityB->m_Id + ":" + entityA->m_Id;

                // Verificar si esta combinación está EN COOLDOWN
                if (m_CollisionCooldowns.find(collisionKey) == m_CollisionCooldowns.end())
                {
                    // COOLDOWN NO ACTIVO - PROCESAR COLISIÓN ENTRE ENEMIGOS

                    // Calcular centros geométricos de ambos enemigos
                    // Centro = posición + (bounds / 2) → punto medio del rectángulo
                    glm::vec2 centerA = transformA->m_Position + (colliderA->m_Bounds * 0.5f);
                    glm::vec2 centerB = transformB->m_Position + (colliderB->m_Bounds * 0.5f);

                    // Calcular dirección de separación
                    // Vector desde B hacia A (dirección de empuje para A)
                    glm::vec2 pushDirection = glm::normalize(centerA - centerB);

                    // Empujar ambos enemigos en direcciones opuestas
                    // Esto evita que se atraviesen mutuamente
                    float pushStrength = 10.0f;
                    transformA->m_Position += pushDirection * pushStrength; // Empujar A hacia afuera
                    transformB->m_Position -= pushDirection * pushStrength; // Empujar B hacia adentro

                    // Invertir velocidades de ambos enemigos (efecto de rebote)
                    // Si A iba hacia (50, 0) ahora va a (-50, 0)
                    // Si B iba hacia (-30, 20) ahora va a (30, -20)
                    transformA->m_Velocity *= -1.0f;
                    transformB->m_Velocity *= -1.0f;

                    // ESTABLECER COOLDOWN de 0.5 segundos
                    // Igual que con el jugador, evita procesamiento repetido
                    m_CollisionCooldowns[collisionKey] = m_CooldownTime;
                }
                // Si está en cooldown: Se ignora esta colisión
            }
        }
    }

    // 3. Verificar colisiones con los límites del mundo
    checkWorldBoundaries(entities);
}
