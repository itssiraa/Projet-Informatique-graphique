#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "OrbitCamera.h"
#include "node.h"
#pragma once

class BoatAnimator;

class Viewer {
public:
    Viewer(int width = 1500, int height = 1500);

    void run();
    void updateBoat(float deltaTime);
    void draw_skybox(glm::mat4 view, glm::mat4 projection);

    BoatAnimator* animator = nullptr;
    void setAnimator(BoatAnimator* a) { animator = a;  }

    // Pour la gestion du temps
    float lastFrameTime = 0.0f;

    OrbitCamera* orbitCamera = nullptr;  // nouvelle caméra orbitale


    // Initialiser ces variables après construction
    Shader* skyboxShader;
    GLuint skyboxVAO;
    GLuint cubemapTexture;



    Node* scene_root;

    
    
  

    // Pour la gestion du temps
  
    Node* boatTransformNode = nullptr;
    glm::vec3 boatPosition = glm::vec3(-0.5f, 0.0f, -4.0f);
    float boatDirection = 0.0f;
    float boatSpeed = 1.5f;


    static void cursor_position_callback_static(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback_static(GLFWwindow* window, double xoffset, double yoffset);
    void on_mouse_move(double xpos, double ypos);
    void on_scroll(double yoffset);

    static void mouse_button_callback_static(GLFWwindow* window, int button, int action, int mods);
    void on_mouse_button(int button, int action);

    bool mousePressed = false;
    double lastX = 0.0, lastY = 0.0;

    bool orbitMode = false; // Par défaut désactivé




private:

    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
    GLFWwindow* win;
    void on_key(int key, int action);



};

#endif // VIEWER_H