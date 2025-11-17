#include "Game.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "HealthComponent.h"
#include "EnemyComponent.h"
#include <fstream>
#include <json.hpp>

using json = nlohmann::json;

Game::Game()
    : m_Window(nullptr), m_Renderer(nullptr), m_Ancho(800), m_Alto(600), m_IsRunning(false), m_GameOver(false), m_World()
{
}

bool Game::Initialize()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        spdlog::error("SDL_Init error: {}", SDL_GetError());
        m_IsRunning = false;
        return false;
    }
    // SDL3: crear ventana (firma: title, width, height, flags)
    m_Window = SDL_CreateWindow("Game", static_cast<int>(m_Ancho), static_cast<int>(m_Alto), SDL_WINDOW_FULLSCREEN);
    if (!m_Window)
    {
        spdlog::error("SDL_CreateWindow error: {}", SDL_GetError());
        m_IsRunning = false;
        return false;
    }

    // SDL3: crear renderer (driver name or nullptr)
    m_Renderer = SDL_CreateRenderer(m_Window, nullptr);
    if (!m_Renderer)
    {
        spdlog::error("SDL_CreateRenderer error: {}", SDL_GetError());
        m_IsRunning = false;
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
        return false;
    }

    m_IsRunning = true; // si todo va bien

    // Leer configuración desde data.json
    float spawnInterval = 2.5f; // Valor por defecto
    try
    {
        std::ifstream configFile("./assets/data.json");
        if (configFile.is_open())
        {
            json config = json::parse(configFile);
            spawnInterval = config.value("spawnInterval", 2.5f);
            spdlog::info("Configuracion cargada: spawnInterval = {:.2f}s", spawnInterval);
        }
        else
        {
            spdlog::warn("No se pudo abrir data.json, usando valores por defecto");
        }
    }
    catch (const std::exception &e)
    {
        spdlog::error("Error al leer data.json: {}", e.what());
    }

    // Inicializar sistemas ECS
    m_PlayerInputSystem = std::make_unique<PlayerInputSystem>(m_Ancho, m_Alto, 200.0f);
    m_MovementSystem = std::make_unique<MovementSystem>();
    m_RenderSystem = std::make_unique<RenderSystem>(m_Renderer);
    m_CollisionSystem = std::make_unique<CollisionSystem>();
    m_CollisionSystem->SetWorldDimensions(m_Ancho, m_Alto); // Configurar dimensiones del mundo
    m_DamageSystem = std::make_unique<DamageSystem>(&m_IsRunning, &m_GameOver);
    m_SpawnSystem = std::make_unique<SpawnSystem>(m_Renderer, m_Ancho, m_Alto, spawnInterval);
    m_EnemyAISystem = std::make_unique<EnemyAISystem>();

    spdlog::info("Sistemas ECS inicializados correctamente");

    return true;
}

void Game::Start()
{
    // Crear el jugador (Megaman)
    Entity &player = m_World.createEntity();

    auto playerComp = std::make_unique<PlayerComponent>();
    player.AddComponent(std::move(playerComp));

    auto playerTransform = std::make_unique<TransformComponent>(m_Ancho / 2.0f, m_Alto / 2.0f, 0.0f, 0.0f);
    player.AddComponent(std::move(playerTransform));

    auto playerSprite = std::make_unique<SpriteComponent>("./assets/megaman.png", m_Renderer);
    player.AddComponent(std::move(playerSprite));

    auto playerCollider = std::make_unique<ColliderComponent>(30.0f, 34.0f);
    player.AddComponent(std::move(playerCollider));

    auto playerHealth = std::make_unique<HealthComponent>(100);
    player.AddComponent(std::move(playerHealth));

    spdlog::info("Jugador creado: ID={}, Posicion=({:.1f},{:.1f})",
                 player.m_Id, m_Ancho / 2.0f, m_Alto / 2.0f);
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            m_IsRunning = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (!event.key.repeat)
            {
                // Si está en Game Over, Enter o ESC salen
                if (m_GameOver)
                {
                    if (event.key.key == SDLK_RETURN || event.key.key == SDLK_ESCAPE)
                    {
                        m_IsRunning = false;
                    }
                }
                else
                {
                    // Durante el juego, solo ESC sale
                    if (event.key.key == SDLK_ESCAPE)
                    {
                        m_IsRunning = false;
                    }
                }
            }
            break;
        }
    }
}

void Game::Update(float deltaTime)
{
    // Si está en Game Over, no actualizar la lógica del juego
    if (m_GameOver)
        return;

    if (m_SpawnSystem)
        m_SpawnSystem->update(m_World, deltaTime);

    if (m_PlayerInputSystem)
        m_PlayerInputSystem->update(m_World, deltaTime);

    if (m_EnemyAISystem)
        m_EnemyAISystem->update(m_World, deltaTime);

    if (m_MovementSystem)
        m_MovementSystem->update(m_World, deltaTime);

    if (m_CollisionSystem)
        m_CollisionSystem->update(m_World, deltaTime);

    if (m_DamageSystem)
        m_DamageSystem->update(m_World, deltaTime);
}

void Game::Render()
{
    if (m_RenderSystem)
    {
        if (m_GameOver)
        {
            // Renderizar pantalla de Game Over
            SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255); // Fondo negro
            SDL_RenderClear(m_Renderer);

            // Mostrar tiempo final (el RenderSystem ya tiene el tiempo acumulado)
            float finalTime = m_RenderSystem->getElapsedTime();

            // Nota: Para mostrar texto necesitarías SDL_ttf, pero por ahora
            // solo dibujamos el fondo negro y el log muestra el tiempo
            spdlog::info("=== GAME OVER ===");
            spdlog::info("Tiempo Final: {:.2f} segundos", finalTime);
            spdlog::info("Presiona ENTER o ESC para salir");

            SDL_RenderPresent(m_Renderer);
        }
        else
        {
            // Renderizado normal del juego
            m_RenderSystem->update(m_World, 0.0f);
        }
    }
}

void Game::Run()
{
    auto millisecsPreviousFrame = SDL_GetTicks();
    while (m_IsRunning)
    {
        ProcessInput();
        double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
        Update(deltaTime);
        millisecsPreviousFrame = SDL_GetTicks();
        Render();
    }
}

Game::~Game()
{
    if (m_Renderer)
    {
        SDL_DestroyRenderer(m_Renderer);
        m_Renderer = nullptr;
    }
    if (m_Window)
    {
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
    }
    SDL_Quit();
}
