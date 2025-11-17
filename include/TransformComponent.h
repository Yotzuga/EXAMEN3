#pragma once
#include "Component.h"
#include <glm/vec2.hpp>

class TransformComponent : public Component
{
public:
    glm::vec2 m_Position; // Posición en el mundo (x, y)
    glm::vec2 m_Velocity; // Velocidad de movimiento (vx, vy)

    TransformComponent(float x, float y, float vx, float vy);
};
