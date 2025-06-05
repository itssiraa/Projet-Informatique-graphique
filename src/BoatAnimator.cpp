#include "BoatAnimator.h"
#include "viewer.h"
#include "node.h"
#include <glm/gtc/matrix_transform.hpp>

BoatAnimator::BoatAnimator(Node* node)
    : boatNode(node), position(0.0f), speed(1.0f), angleDeg(0.0f) {
}

void BoatAnimator::setSpeed(float s) {
    speed = s;
}

void BoatAnimator::setDirection(float angleDegrees) {
    angleDeg = angleDegrees;
}

void BoatAnimator::update(float deltaTime) {
    if (!moving) return;
    timeAccumulator += deltaTime;

    // Avance dans la direction actuelle
    float angleRad = glm::radians(direction);
    glm::vec3 forward = glm::vec3(cos(angleRad), 0.0f, sin(angleRad));
    position += forward * speed * deltaTime;

    // --- TANGAGE ---
    float tiltAngle = glm::radians(1.5f) * sin(timeAccumulator * 2.0f); // ±2.5° à 2 Hz
    glm::mat4 tilt = glm::rotate(glm::mat4(1.0f), tiltAngle, glm::vec3(1, 0, 0));

    // Transformation combinée : translation + rotation + tangage
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), glm::radians(-direction), glm::vec3(0, 1, 0))
        * tilt;
    // Mise à jour de la transformation
    //glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(-direction), glm::vec3(0, 1, 0));

    boatNode->set_transform(transform);
}

void BoatAnimator::handleKey(int key, int action) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_SPACE) {
            moving = !moving;
        }
    }

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_LEFT) {
            direction -= turnSpeed * 0.016f; // tourner à gauche (approx. 60 FPS)
        }
        if (key == GLFW_KEY_RIGHT) {
            direction += turnSpeed * 0.016f; // tourner à droite
        }
    }
}