#pragma once
#include "Component.h"
#include <glm/vec2.hpp>

class ColliderComponent : public Component
{
public:
    glm::vec2 m_Bounds; // Tamaño del rectángulo de colisión (ancho, alto)

    ColliderComponent(float width, float height);
};
