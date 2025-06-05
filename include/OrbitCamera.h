#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OrbitCamera {
public:
    OrbitCamera(glm::vec3 target, float distance);

    void rotate(float deltaX, float deltaY);
    void zoom(float deltaZoom);
    void update();  // Recalcul la matrice view
    glm::vec3 getPosition() const { return position; }


    glm::mat4 getViewMatrix() const;

    void setTarget(const glm::vec3& newTarget);

private:
    glm::vec3 position;
    glm::vec3 target_;
    float distance_;
    float yaw_;   // en degrés
    float pitch_; // en degrés
    glm::mat4 viewMatrix_;

    void clampAngles();
};
