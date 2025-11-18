#include "PlayerInputSystem.h"
#include "Entity.h"
#include "PlayerComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include <algorithm>

PlayerInputSystem::PlayerInputSystem(float worldWidth, float worldHeight, float playerSpeed)
    : m_PlayerSpeed(playerSpeed), m_WorldWidth(worldWidth), m_WorldHeight(worldHeight), m_KeyState(nullptr)
{
}

void PlayerInputSystem::updateKeyState()
{
    m_KeyState = SDL_GetKeyboardState(nullptr);
}

void PlayerInputSystem::update(World &world, float dt)
{
    // Actualizar estado del teclado
    updateKeyState();

    if (!m_KeyState)
        return;

    // Buscar entidad del jugador
    auto entities = world.GetEntities();
    for (Entity *e : entities)
    {
        auto *player = e->GetComponent<PlayerComponent>();
        if (!player)
            continue; // No es el jugador

        auto *transform = e->GetComponent<TransformComponent>();
        auto *collider = e->GetComponent<ColliderComponent>();

        if (!transform)
            continue;

        // Calcular velocidad basada en input WASD
        glm::vec2 velocity(0.0f, 0.0f);

        if (m_KeyState[SDL_SCANCODE_W])
        {
            velocity.y -= m_PlayerSpeed;
        }
        if (m_KeyState[SDL_SCANCODE_S])
        {
            velocity.y += m_PlayerSpeed;
        }
        if (m_KeyState[SDL_SCANCODE_A])
        {
            velocity.x -= m_PlayerSpeed;
        }
        if (m_KeyState[SDL_SCANCODE_D])
        {
            velocity.x += m_PlayerSpeed;
        }

        // Aplicar velocidad
        transform->m_Velocity = velocity;

        // Requerimiento de PREGUNTA 2 – Movimientos, input y renderizado (4 puntos)
        // El void CollisionSystem::checkWorldBoundaries() se encarga de que el jugador y enemigo no se salga de los límites del mundo.
        // Por lo tanto, estaria redundando aqui.
        /*if (collider)
        {
            // Clamp posición X
            transform->m_Position.x = std::clamp(transform->m_Position.x,
                                                 0.0f,
                                                 m_WorldWidth - collider->m_Bounds.x);
            // Clamp posición Y
            transform->m_Position.y = std::clamp(transform->m_Position.y,
                                                 0.0f,
                                                 m_WorldHeight - collider->m_Bounds.y);
        }*/
    }
}
