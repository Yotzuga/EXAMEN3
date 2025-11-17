#pragma once
#include "ISystem.h"
#include "World.h"
#include <SDL3/SDL.h>

class PlayerInputSystem : public ISystem
{
private:
    float m_PlayerSpeed;    // Velocidad del jugador
    float m_WorldWidth;     // Ancho del mundo
    float m_WorldHeight;    // Alto del mundo
    const bool *m_KeyState; // Estado del teclado (SDL3 usa bool)

public:
    PlayerInputSystem(float worldWidth, float worldHeight, float playerSpeed = 200.0f);

    void update(World &world, float dt) override;
    void updateKeyState(); // Actualiza el estado del teclado
};
