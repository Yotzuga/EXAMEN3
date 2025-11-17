#include "Component.h"

int Component::m_Cantidad = 0;

std::string Component::GenerateComponentId()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<unsigned int> dis(0, 0xFFFFFFFF);

    std::stringstream ss;
    ss << "C_" << std::hex << std::setfill('0') << std::setw(8) << dis(gen);
    return ss.str();
}

Component::Component() : m_Id(GenerateComponentId()), m_Type("Component")
{
    m_Cantidad++;
}

// Getters y destructor
const std::string &Component::getId() const
{
    return m_Id;
}

const std::string &Component::getType() const
{
    return m_Type;
}

int Component::getCantidad()
{
    return m_Cantidad;
}

Component::~Component()
{
    m_Cantidad--;
}