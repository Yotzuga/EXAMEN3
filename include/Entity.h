#pragma once
#include <string>
#include <iostream>
#include "Component.h"
#include <vector>
#include <memory>

class Entity
{
private:
    std::vector<std::unique_ptr<Component>> m_Components; // Lista de componentes asociados

public:
    std::string m_Id;      // Identificador único
    std::string m_Name;    // Nombre simbólico
    static int m_Cantidad; // Número (opcional) asociado a la instancia.
    Entity();
    Entity(const std::string &id, const std::string &name);
    ~Entity();

    void AddComponent(std::unique_ptr<Component> component); // Agrega un componente a la entidad.
    Component *GetComponent(const std::string &type) const;  // Busca y devuelve un componente por type (DEPRECATED - usar template)

    // Metodo template para obtener componente por tipo
    template <typename T>
    T *GetComponent() const
    {
        for (const auto &comp : m_Components)
        {
            T *casted = dynamic_cast<T *>(comp.get());
            if (casted)
                return casted;
        }
        return nullptr;
    }

    std::string getId() const;
    std::string getName() const;
};
