#include "Entity.h"
#include "Component.h"

Entity::Entity() : m_Id(""), m_Name("")
{                 // Constructor por defecto No usado al final de  todo
    m_Cantidad++; // opcional
}

Entity::Entity(const std::string &id, const std::string &name) : m_Id(id), m_Name(name)
{ // Constructor parametrizado NO REQUERIDO
    m_Cantidad++;
}

void Entity::AddComponent(std::unique_ptr<Component> component) // Agrega un componente a la entidad.
{
    m_Components.push_back(std::move(component));
}

Component *Entity::GetComponent(const std::string &type) const
{ // Busca por m_Type en lugar de m_Id
    for (const auto &component : m_Components)
    {
        if (component->m_Type == type)
        {
            return component.get(); // Devuelve puntero raw (no-owning)
        }
    }
    return nullptr; // No encontrado
}

int Entity::m_Cantidad = 0; // no usado

// Getters y destructor
std::string Entity::getId() const
{
    return m_Id;
}
std::string Entity::getName() const
{
    return m_Name;
}
Entity::~Entity()
{
    // Los unique_ptr liberan automáticamente la memoria
    m_Components.clear();
    m_Cantidad--;
}