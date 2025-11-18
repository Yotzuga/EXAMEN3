#include "RenderSystem.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include <spdlog/spdlog.h>
#include <sstream>
#include <iomanip>

RenderSystem::RenderSystem(SDL_Renderer *renderer)
    : m_Renderer(renderer), m_ElapsedTime(0.0f)
{
    m_StartTime = std::chrono::steady_clock::now(); // Inicializar el tiempo de inicio
}

void RenderSystem::resetTimer()
{
    m_StartTime = std::chrono::steady_clock::now(); // Reiniciar el tiempo de inicio
    m_ElapsedTime = 0.0f;
}

float RenderSystem::getElapsedTime() const
{
    return m_ElapsedTime;
}

void RenderSystem::update(World &world, float dt)
{
    if (!m_Renderer)
        return;

    // Actualizar tiempo transcurrido
    auto currentTime = std::chrono::steady_clock::now();
    m_ElapsedTime = std::chrono::duration<float>(currentTime - m_StartTime).count();

    // Limpiar pantalla
    SDL_SetRenderDrawColor(m_Renderer, 20, 20, 40, 255); // Fondo oscuro azulado
    SDL_RenderClear(m_Renderer);

    // Dibujar todas las entidades con sprite
    auto entities = world.GetEntities();
    for (Entity *e : entities)
    {
        auto *transform = e->GetComponent<TransformComponent>();
        auto *sprite = e->GetComponent<SpriteComponent>();
        auto *collider = e->GetComponent<ColliderComponent>();

        float width = collider ? collider->m_Bounds.x : 32.0f;
        float height = collider ? collider->m_Bounds.y : 32.0f;

        SDL_FRect dstRect{transform->m_Position.x, transform->m_Position.y, width, height};
        SDL_RenderTexture(m_Renderer, sprite->m_Texture, nullptr, &dstRect);
    }

    SDL_RenderPresent(m_Renderer);
}
