#pragma once
#include "ISystem.h"
#include "World.h"
#include <SDL3/SDL.h>
#include <chrono>

class RenderSystem : public ISystem
{
private:
    SDL_Renderer *m_Renderer;
    std::chrono::steady_clock::time_point m_StartTime;
    float m_ElapsedTime; // Tiempo transcurrido en segundos

public:
    RenderSystem(SDL_Renderer *renderer);

    void update(World &world, float dt) override;
    void resetTimer();            // Reinicia el timer
    float getElapsedTime() const; // Obtiene tiempo transcurrido
};
