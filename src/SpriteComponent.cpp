#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(const std::string &filePath, SDL_Renderer *renderer)
    : Component(), m_Texture(nullptr)
{
    m_Type = "SpriteComponent";
    // Cargar la textura usando SDL_image
    m_Texture = IMG_LoadTexture(renderer, filePath.c_str());
    if (!m_Texture)
    {
        spdlog::error("Error al cargar la textura: {}", filePath);
    }
}

SpriteComponent::~SpriteComponent()
{ // Liberar la textura al destruir el componente
    if (m_Texture)
    {
        SDL_DestroyTexture(m_Texture);
        m_Texture = nullptr;
    }
}