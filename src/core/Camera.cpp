//
// Created by Antonin Fruchet on 05/08/2026.
//

#include "Camera.h"

#include <cmath>

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

void Camera::move(float forward, float rightMove, float upMove, float dt)
{
    float velocity = speed * dt;

    sf::Glsl::Vec3 rightDir{ -dir.z, 0.0f, dir.x };
    float rLen = std::sqrt(rightDir.x * rightDir.x + rightDir.z * rightDir.z);
    if (rLen > 0.0001f) {
        rightDir.x /= rLen;
        rightDir.z /= rLen;
    }

    sf::Glsl::Vec3 worldUp{ 0.0f, 1.0f, 0.0f };

    pos.x += (dir.x * forward + rightDir.x * rightMove + worldUp.x * upMove) * velocity;
    pos.y += (dir.y * forward + rightDir.y * rightMove + worldUp.y * upMove) * velocity;
    pos.z += (dir.z * forward + rightDir.z * rightMove + worldUp.z * upMove) * velocity;
}



