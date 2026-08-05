//
// Created by Antonin Fruchet on 05/08/2026.
//

#include "Camera.h"

#include "Inputs/InputManager.h"

Camera::Camera(sf::Glsl::Vec3 pos, sf::Glsl::Vec3 dir, float focal_length)
    : pos(pos), dir(dir), focal_length(focal_length)
{
    // Calcul automatique du yaw et pitch initiaux depuis la direction passée au constructeur
    pitch = std::asin(dir.y) * 180.0f / 3.14159265f;
    yaw = std::atan2(dir.z, dir.x) * 180.0f / 3.14159265f;
    updateVectors();
}

void Camera::updateForShader(sf::Shader* shader)
{
    shader->setUniform("u_cam.pos", pos);
    shader->setUniform("u_cam.dir", dir);
    shader->setUniform("u_cam.focal", focal_length);
}

void Camera::updateVectors()
{
    float radYaw = yaw * 3.14159265f / 180.0f;
    float radPitch = pitch * 3.14159265f / 180.0f;

    // Calcul du nouveau vecteur direction 3D
    dir.x = std::cos(radYaw) * std::cos(radPitch);
    dir.y = std::sin(radPitch);
    dir.z = std::sin(radYaw) * std::cos(radPitch);

    // Normalisation du vecteur
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    if (len > 0.0f) {
        dir.x /= len;
        dir.y /= len;
        dir.z /= len;
    }
}

void Camera::look(float deltaX, float deltaY)
{
    yaw += deltaX * sensitivity;
    pitch -= deltaY * sensitivity;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateVectors();
}

void Camera::move(const InputAction* moveAction, const InputAction* jumpAction, const InputAction* crouchAction, const sf::Time& elapsedTime)
{
    auto flat_movement = moveAction->ReadValue<sf::Vector2f>();
    auto up_move = jumpAction->ReadValue<bool>() - crouchAction->ReadValue<bool>();
    float velocity = speed * elapsedTime.asSeconds();

    sf::Glsl::Vec3 rightDir{ -dir.z, 0.0f, dir.x };
    float rLen = std::sqrt(rightDir.x * rightDir.x + rightDir.z * rightDir.z);
    if (rLen > 0.0001f) {
        rightDir.x /= rLen;
        rightDir.z /= rLen;
    }

    sf::Glsl::Vec3 worldUp{ 0.0f, 1.0f, 0.0f };

    pos.x += (dir.x * -flat_movement.y + rightDir.x * flat_movement.x + worldUp.x * up_move) * velocity;
    pos.y += (dir.y * -flat_movement.y + rightDir.y * flat_movement.x + worldUp.y * up_move) * velocity;
    pos.z += (dir.z * -flat_movement.y + rightDir.z * flat_movement.x + worldUp.z * up_move) * velocity;
}



