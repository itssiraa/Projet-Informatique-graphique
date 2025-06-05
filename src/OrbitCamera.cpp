#include "OrbitCamera.h"
#include <glm/gtx/transform.hpp>
#include <algorithm>

OrbitCamera::OrbitCamera(glm::vec3 target, float distance)
    : target_(target), distance_(distance), yaw_(90.0f), pitch_(20.0f)
{
    update();
}

void OrbitCamera::rotate(float deltaX, float deltaY)
{
    yaw_ += deltaX;
    pitch_ += deltaY;
    clampAngles();
    update();
}

void OrbitCamera::zoom(float deltaZoom)
{
    distance_ += deltaZoom;
    if (distance_ < 2.0f) distance_ = 2.0f;
    if (distance_ > 30.0f) distance_ = 30.0f;
    update();
}

void OrbitCamera::setTarget(const glm::vec3& newTarget)
{
    target_ = newTarget;
    update();
}

void OrbitCamera::clampAngles()
{
    if (pitch_ < -89.0f) pitch_ = -89.0f;
    else if (pitch_ > 89.0f) pitch_ = 89.0f;
}

void OrbitCamera::update()
{
    float yawRad = glm::radians(yaw_);
    float pitchRad = glm::radians(pitch_);

    glm::vec3 offset;
    offset.x = distance_ * cos(pitchRad) * cos(yawRad);
    offset.y = distance_ * sin(pitchRad);
    offset.z = distance_ * cos(pitchRad) * sin(yawRad);

    glm::vec3 position = target_ + offset;
    viewMatrix_ = glm::lookAt(position, target_, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 OrbitCamera::getViewMatrix() const
{
    return viewMatrix_;
}