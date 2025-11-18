#include "World.h"
#include "Component.h"
#include "Entity.h"
#include <iostream>
#include <spdlog/spdlog.h>

World::World() = default;

void World::AddEntity(std::unique_ptr<Entity> entity)
{
    m_Entities.push_back(std::move(entity));
}

void World::AddComponentToEntity(const std::string &entityId, std::unique_ptr<Component> component)
{ // Añade un componente a una entidad específica.
    for (auto &entity : m_Entities)
    {
        if (entity->getId() == entityId)
        {
            entity->AddComponent(std::move(component));
            return;
        }
    }
}

Entity *World::GetEntityByName(const std::string &name) const
{ // Busca una entidad por nombre
    for (const auto &entity : m_Entities)
    {
        if (entity->getName() == name)
        {
            return entity.get(); // Devuelve puntero raw (non-owning)
        }
    }
    return nullptr; // Si no se encuentra la entidad, devuelve nullptr
}

std::vector<Entity *> World::GetEntities() const
{ // Devuelve vector de punteros entityList para iteración
    std::vector<Entity *> entityList;
    entityList.reserve(m_Entities.size());
    for (const auto &entity : m_Entities)
    {
        entityList.push_back(entity.get());
    }
    return entityList;
}

// Crea una entidad con ID y nombre únicos
Entity &World::createEntity()
{
    static int entityCounter = 0;
    std::string id = "E" + std::to_string(entityCounter);
    std::string name = "Entity" + std::to_string(entityCounter);
    entityCounter++;

    auto entity = std::make_unique<Entity>(id, name);
    Entity *entityPtr = entity.get();
    m_Entities.push_back(std::move(entity));

    spdlog::debug("Entidad {} creada en World", id);
    return *entityPtr;
}

// Emite un evento al bus
void World::emit(std::unique_ptr<Event> event)
{
    spdlog::debug("Evento emitido: {}", event->getType());
    m_EventQueue.push(std::move(event));
}

// Obtiene el siguiente evento de la cola (FIFO)
bool World::poll(std::unique_ptr<Event> &out)
{
    if (m_EventQueue.empty())
    {
        return false;
    }
    out = std::move(m_EventQueue.front());
    m_EventQueue.pop();
    return true;
}

World::~World()
{
    // Los unique_ptr liberan automáticamente la memoria
    m_Entities.clear();

    // Limpiar cola de eventos
    while (!m_EventQueue.empty())
    {
        m_EventQueue.pop();
    }
}
