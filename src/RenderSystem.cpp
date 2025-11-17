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
    m_StartTime = std::chrono::steady_clock::now();
}

void RenderSystem::resetTimer()
{
    m_StartTime = std::chrono::steady_clock::now();
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

        if (!transform || !sprite)
            continue;

        // Determinar tamaño de renderizado
        float width = 32.0f; // Tamaño por defecto
        float height = 32.0f;

        if (collider)
        {
            width = collider->m_Bounds.x;
            height = collider->m_Bounds.y;
        }

        SDL_FRect dstRect{transform->m_Position.x, transform->m_Position.y, width, height};

        if (sprite->m_Texture)
        {
            SDL_RenderTexture(m_Renderer, sprite->m_Texture, nullptr, &dstRect);
        }
        else
        {
            // Dibujar rectángulo si no hay textura
            SDL_SetRenderDrawColor(m_Renderer, 200, 100, 100, 255);
            SDL_RenderRect(m_Renderer, &dstRect);
        }
    }

    // Dibujar tiempo transcurrido (como texto simple usando rectángulos por ahora)
    // Nota: Para texto real necesitarías SDL_ttf, por ahora usamos logging
    // Formatear tiempo como MM:SS.mmm
    int minutes = static_cast<int>(m_ElapsedTime) / 60;
    int seconds = static_cast<int>(m_ElapsedTime) % 60;
    int milliseconds = static_cast<int>((m_ElapsedTime - static_cast<int>(m_ElapsedTime)) * 1000);

    // TODO: Renderizar texto en pantalla (requiere SDL_ttf)
    // Por ahora, el tiempo se puede consultar con getElapsedTime()

    SDL_RenderPresent(m_Renderer);
}
