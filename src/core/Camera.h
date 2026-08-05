//
// Created by Antonin Fruchet on 05/08/2026.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H
#include "Inputs/InputAction.h"
#include "SFML/Graphics.hpp"


class Camera {
    sf::Glsl::Vec3 pos;
    sf::Glsl::Vec3 dir;
    float focal_length;

    float yaw = -90.0f; // Angles d'Euler (orientation de départ vers -Z)
    float pitch = 0.0f;

    float speed = 4.0f;        // Vitesse de déplacement
    float sensitivity = 0.15f; // Sensibilité de la souris

    void updateVectors();
public:
    Camera(sf::Glsl::Vec3 pos, sf::Glsl::Vec3 dir, float focal_length);

    void updateForShader(sf::Shader* shader);

    // Mouvement continu (Clavier)
    void move(const InputAction* moveAction, const InputAction* jumpAction, const InputAction* crouchAction, const sf::Time& elapsedTime);
    // Orientation (Souris)
    void look(float deltaX, float deltaY);
};



#endif //RAYTRACING_CAMERA_H
