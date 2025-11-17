# Examen2 

## Resumen
Proyecto MEGAMAN con las clases principales: `Game`, `World`, `Entity` y `Component` (y componentes derivados `TransformComponent`, `SpriteComponent`, `ColliderComponent`). El juego crea entidades, las mueve con un `TransformComponent`, dibuja usando `SpriteComponent` y comprueba colisiones con `ColliderComponent`.

## Estructura de carpetas
- `include/` - headers de las clases
- `src/` - implementación (.cpp)
- `assets/` - imágenes usadas por sprites
- `external/` - librerías de terceros (GLM, SDL3, spdlog)

## Flujo del programa

Flujo principal: Game::Start() -> InicializarEntidades() -> Game::Run() -> [ProcessInput(), Update(dt), Render()]

- `Game::Initialize()` inicializa SDL y el renderer.
- `Game::Start()` crea entidades y añade componentes (sprite, transform, collider).
- `Game::Run()` ejecuta el bucle principal: `ProcessInput()`, `Update(deltaTime)` y `Render()`.
- `Update()` mueve las entidades (`transform->m_Position += velocity * dt`) y llama a los sistemas de colisión (`Fisicas::VerificarColisionesEntidades` y `Fisicas::VerificarColisionesWorld`).

## Cómo cambiar parámetros
- Número de entidades: modifica las llamadas a `InicializarEntidades(...)` en `src/Game.cpp` dentro de `Start()`.
- Tamaño de entidades: los parámetros `anchoEntidad` y `altoEntidad` en `InicializarEntidades` controlan el collider y el render.
- Rutas de assets: los strings de ruta en `Start()` (p. ej. `./assets/megaman.png`).


## Cómo compilar y ejecutar
1. Asegúrate de tener las dependencias externas spdlog, sdl3 y glm, estar en la ruta correcta del proyecto.
2. Ejecutar `make` y luego el binario en `make run`.
3. Observa el log en consola para mensajes `spdlog` sobre carga de texturas o creación de entidades.

