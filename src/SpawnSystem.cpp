#include "SpawnSystem.h"
#include "Entity.h"
#include "EnemyComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "HealthComponent.h"
#include <spdlog/spdlog.h>

SpawnSystem::SpawnSystem(SDL_Renderer *renderer, float worldWidth, float worldHeight, float spawnInterval)
    : m_Renderer(renderer), m_WorldWidth(worldWidth), m_WorldHeight(worldHeight),
      m_SpawnInterval(spawnInterval), m_TimeSinceLastSpawn(0.0f),
      m_Rng(std::random_device{}()),
      m_EnemySprite1("./assets/Enemigo1.png"),
      m_EnemySprite2("./assets/Enemigo2.png")
{
}

void SpawnSystem::update(World &world, float dt)
{
    m_TimeSinceLastSpawn += dt;

    // Verificar si es tiempo de spawnear
    if (m_TimeSinceLastSpawn >= m_SpawnInterval)
    {
        // Contar enemigos actuales
        int enemyCount = 0;
        auto entities = world.GetEntities();
        for (Entity *e : entities)
        {
            auto *enemyComp = e->GetComponent<EnemyComponent>();
            if (enemyComp)
                enemyCount++;
        }

        // Solo spawnear si hay menos de 10 enemigos
        if (enemyCount < 10)
        {
            spawnEnemy(world);
            spdlog::info("Enemigos actuales: {}/10", enemyCount + 1);
        }
        else
        {
            spdlog::warn("Limite de enemigos alcanzado (10/10). No se spawneara mas.");
        }

        m_TimeSinceLastSpawn = 0.0f; // Resetear el temporizador
    }
}

void SpawnSystem::spawnEnemy(World &world)
{
    // ---------------------Crear el enemigo---------------------//
    Entity &enemy = world.createEntity();
    // %%%Uso de move() para mover ownership de los componentes a la entidad (m_Components)%%%

    // EnemyComponent
    auto enemyComp = std::make_unique<EnemyComponent>();
    enemy.AddComponent(std::move(enemyComp));

    // Elegir sprite aleatorio y tamaño correspondiente
    std::uniform_int_distribution<int> spriteDist(0, 1);
    bool useSprite1 = (spriteDist(m_Rng) == 0);

    // Sprite y tamaño según elección
    std::string spriteAsset = useSprite1 ? m_EnemySprite1 : m_EnemySprite2;
    float width = useSprite1 ? 30.0f : 24.0f;
    float height = useSprite1 ? 50.0f : 29.0f;

    // SpriteComponent del enemigo
    auto sprite = std::make_unique<SpriteComponent>(spriteAsset, m_Renderer);
    enemy.AddComponent(std::move(sprite));

    // ColliderComponent del enemigo
    auto collider = std::make_unique<ColliderComponent>(width, height);
    enemy.AddComponent(std::move(collider));

    // TransformComponent con posición y velocidad aleatoria
    std::uniform_real_distribution<float> posXDist(0.0f, std::max(0.0f, m_WorldWidth - width));
    std::uniform_real_distribution<float> posYDist(0.0f, std::max(0.0f, m_WorldHeight - height));
    std::uniform_real_distribution<float> velDist(-80.0f, 80.0f);

    float px = posXDist(m_Rng);
    float py = posYDist(m_Rng);
    float vx = velDist(m_Rng);
    float vy = velDist(m_Rng);

    auto transform = std::make_unique<TransformComponent>(px, py, vx, vy);
    enemy.AddComponent(std::move(transform));

    spdlog::debug("Enemigo spawneado: ID={}, Pos=({:.1f},{:.1f}), Vel=({:.1f},{:.1f})",
                  enemy.m_Id, px, py, vx, vy);
}
