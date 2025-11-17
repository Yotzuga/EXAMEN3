#pragma once
#include "ISystem.h"
#include "World.h"

class DamageSystem : public ISystem
{
private:
    bool *m_GameRunning; // Puntero al flag del juego para terminarlo si el jugador muere
    bool *m_GameOver;    // Puntero al flag de Game Over

public:
    DamageSystem(bool *gameRunning, bool *gameOver);

    void update(World &world, float dt) override;
};
