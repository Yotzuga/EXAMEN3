#pragma once
#include <string>
#include <iostream>
#include <random>
#include <sstream>
#include <iomanip>

class Component
{
private:
    static int m_Cantidad;                    // número de componentes creados (opcional).
    static std::string GenerateComponentId(); // Genera ID único aleatorio

public:
    const std::string m_Id; // Identificador único del componente (generado automáticamente)
    std::string m_Type;     // Tipo del componente (ej: "TransformComponent")

    Component(); // Constructor por defecto (genera ID automático)

    virtual ~Component(); // Destructor virtual para herencia segura.

    const std::string &getId() const;
    const std::string &getType() const;

    static int getCantidad();
};