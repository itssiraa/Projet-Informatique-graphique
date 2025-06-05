#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "node.h"

class Viewer;

class BoatAnimator {
public:
    BoatAnimator(Node* targetNode/*, Viewer* viewer*/);


    void handleKey(int key, int action);
    void update(float deltaTime);
    void setSpeed(float speed);
    void setDirection(float angleDegrees);

private:
    Node* boatNode;
    //Viewer* viewerRef;  // ref vers le viewer
    glm::vec3 position;
    float speed;
    float angleDeg;
    float direction = 0.0f; // en degrés
    float turnSpeed = 90.0f; // degrés/seconde
    bool moving = true;

    float timeAccumulator = 0.0f; // temps cumulé pour l’oscillation
};