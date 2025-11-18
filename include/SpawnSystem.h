#pragma once
#include "ISystem.h"
#include "World.h"
#include <SDL3/SDL.h>
#include <random>
#include <string>

class SpawnSystem : public ISystem
{
private:
    SDL_Renderer *m_Renderer;
    float m_WorldWidth;
    float m_WorldHeight;
    float m_SpawnInterval;      // Tiempo entre spawns (segundos)
    float m_TimeSinceLastSpawn; // Acumulador de tiempo

    std::mt19937 m_Rng;
    std::string m_EnemySprite1;
    std::string m_EnemySprite2;
    int m_CurrentEnemyCount = 0;

public:
    SpawnSystem(SDL_Renderer *renderer, float worldWidth, float worldHeight, float spawnInterval);

    void update(World &world, float dt) override;

    void setSpawnInterval(float interval) { m_SpawnInterval = interval; }

private:
    void spawnEnemy(World &world);
};
