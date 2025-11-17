#pragma once
#include "ISystem.h"
#include "World.h"
#include <glm/glm.hpp>
#include <map>
#include <string>

class CollisionSystem : public ISystem
{
public:
    CollisionSystem();

    void update(World &world, float dt) override;

    // Setter para las dimensiones del mundo (necesario para colisiones con bordes)
    void SetWorldDimensions(float width, float height);

private:
    // Función auxiliar para verificar colisión AABB
    bool checkAABBCollision(const glm::vec2 &pos1, const glm::vec2 &size1,
                            const glm::vec2 &pos2, const glm::vec2 &size2);

    // Verificar colisiones de entidades con los límites del mundo (rebotes en bordes)
    void checkWorldBoundaries(const std::vector<Entity *> &entities);

    // Cooldown para evitar colisiones continuas
    std::map<std::string, float> m_CollisionCooldowns; // Par de IDs -> tiempo restante
    float m_CooldownTime = 0.5f;                       // 0.5 segundos de cooldown

    // Dimensiones del mundo
    float m_WorldWidth = 800.0f;
    float m_WorldHeight = 600.0f;
};
