#pragma once
#include <string>
#include <iostream>
#include "Component.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>
class SpriteComponent : public Component
{
public:
    SDL_Texture *m_Texture; // Textura cargada en memoria

    SpriteComponent(const std::string &filePath, SDL_Renderer *renderer);
    ~SpriteComponent();
};
