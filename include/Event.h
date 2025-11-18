#pragma once
#include <string>
#include <memory>

// Clase base para todos los eventos
class Event
{
public:
    virtual ~Event() = default;
    virtual std::string getType() const = 0; // Identificador del tipo de evento
};

// Evento de daño
class DamageEvent : public Event
{
public:
    std::string entityId; // ID de la entidad que recibió daño
    int damage;           // Cantidad de daño

    DamageEvent(const std::string &id, int dmg);
    std::string getType() const override;
};

// Evento de spawn (creación de entidad)
class SpawnEvent : public Event
{
public:
    std::string entityType; //
    float x, y;             //

    SpawnEvent(const std::string &type, float posX, float posY);
    std::string getType() const override;
};