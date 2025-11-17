#include "TransformComponent.h"
// Representa la posición y velocidad de una entidad
TransformComponent::TransformComponent(float x, float y, float vx, float vy)
    : Component(), m_Position(x, y), m_Velocity(vx, vy)
{
    m_Type = "TransformComponent";
}
