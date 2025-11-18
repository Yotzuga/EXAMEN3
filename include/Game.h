#pragma once
#include <string>
#include <iostream>
#include "World.h"
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>
#include <memory>

// Incluir los sistemas
#include "PlayerInputSystem.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "CollisionSystem.h"
#include "DamageSystem.h"
#include "SpawnSystem.h"
#include "EnemyAISystem.h"

class Game
{
protected:
    SDL_Window *m_Window;     // Ventana principal
    SDL_Renderer *m_Renderer; // Contexto de renderizado
    float m_Ancho;            // Dimensión ancho
    float m_Alto;             // Dimensión alto
    bool m_IsRunning;         // ¿Juego activo?
    bool m_GameOver;          // ¿Juego terminado?
    bool m_GameOverPrinted;   // ¿Ya se imprimió el Game Over?
    World m_World;            // Mundo del juego

    // Sistemas nuevos
    std::unique_ptr<PlayerInputSystem> m_PlayerInputSystem;
    std::unique_ptr<MovementSystem> m_MovementSystem;
    std::unique_ptr<RenderSystem> m_RenderSystem;
    std::unique_ptr<CollisionSystem> m_CollisionSystem;
    std::unique_ptr<DamageSystem> m_DamageSystem;
    std::unique_ptr<SpawnSystem> m_SpawnSystem;
    std::unique_ptr<EnemyAISystem> m_EnemyAISystem;

public:
    Game();
    ~Game();
    bool Initialize();            // Configura SDL, crea la ventana y el renderer.
    void Run();                   // Lanza el bucle principal
    void Start();                 // Inicializa entidades y componentes
    void ProcessInput();          // Procesa eventos
    void Update(float deltaTime); // Actualiza lógica
    void Render();                // Renderiza
};