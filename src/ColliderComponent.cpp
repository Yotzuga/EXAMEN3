#include "ColliderComponent.h"
// Define los límites físicos para la detección de colisiones.
ColliderComponent::ColliderComponent(float width, float height)
    : Component(), m_Bounds(width, height)
{
    m_Type = "ColliderComponent";
}