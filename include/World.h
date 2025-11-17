#pragma once
#include <string>
#include <iostream>
#include "Entity.h"
#include "Event.h"
#include <vector>
#include <memory>
#include <queue>

class World
{
protected:
    std::vector<std::unique_ptr<Entity>> m_Entities; // Entidades activas
    std::queue<std::unique_ptr<Event>> m_EventQueue; // Cola de eventos

public:
    World();

    // Métodos existentes
    void AddEntity(std::unique_ptr<Entity> entity);
    void AddComponentToEntity(const std::string &entityId, std::unique_ptr<Component> component);
    Entity *GetEntityByName(const std::string &name) const;
    std::vector<Entity *> GetEntities() const;

    // Nuevos métodos para ECS
    Entity &createEntity();                  // Crea y devuelve referencia a nueva entidad
    void emit(std::unique_ptr<Event> event); // Emite un evento al bus
    bool poll(std::unique_ptr<Event> &out);  // Obtiene el siguiente evento de la cola

    ~World();
};