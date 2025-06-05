#include "viewer.h"
#include "BoatAnimator.h"
#include <iostream>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Viewer::Viewer(int width, int height)
{
    if (!glfwInit())    // initialize window system glfw
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
    }

    // version hints: create GL window with >= OpenGL 3.3 and core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    win = glfwCreateWindow(width, height, "Viewer", NULL, NULL);

    if (win == NULL) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
    }

    // make win's OpenGL context current; no OpenGL calls can happen before
    glfwMakeContextCurrent(win);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }

    // Set user pointer for GLFW window to this Viewer instance
    glfwSetWindowUserPointer(win, this);

    // register event handlers
    glfwSetKeyCallback(win, key_callback_static);

    // useful message to check OpenGL renderer characteristics
    std::cout << glGetString(GL_VERSION) << ", GLSL "
        << glGetString(GL_SHADING_LANGUAGE_VERSION) << ", Renderer "
        << glGetString(GL_RENDERER) << std::endl;

    // initialize GL by setting viewport and default render characteristics
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    /* tell GL to only draw onto a pixel if the shape is closer to the viewer
    than anything already drawn at that pixel */
    glEnable(GL_DEPTH_TEST); /* enable depth-testing */
    /* with LESS depth-testing interprets a smaller depth value as meaning "closer" */
    glDepthFunc(GL_LEQUAL);     // pour �viter que la skybox soit z-fight�e par des objets au m�me plan
    glDepthMask(GL_FALSE);      // emp�cher l��criture dans le z-buffer

    glfwSetCursorPosCallback(win, cursor_position_callback_static);
    glfwSetScrollCallback(win, scroll_callback_static);
    glfwSetMouseButtonCallback(win, mouse_button_callback_static);
    // Cam�ra orbitale centr�e sur le bateau
    orbitCamera = new OrbitCamera(boatPosition, 8.0f);

    // initialize our scene_root
    scene_root = new Node();
}

void Viewer::run()
{
    lastFrameTime = glfwGetTime();
    // Main render loop for this OpenGL window
    while (!glfwWindowShouldClose(win))
    {
        // initialise le temps de la premi�re frame
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        if (animator) animator->update(deltaTime);
        // clear draw buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*glm::vec3 boatPos = boatPosition;
        float angleRad = glm::radians(boatDirection);

        // D�calage de la cam�ra en arri�re du bateau
        glm::vec3 offset = glm::vec3(-cos(angleRad) * 10.0f, 6.0f, -sin(angleRad) * 10.0f);
        glm::vec3 cameraPos = boatPos + offset;
        glm::vec3 target = boatPos + glm::vec3(cos(angleRad), 0.5f, sin(angleRad));

        glm::mat4 view = glm::lookAt(cameraPos, target, glm::vec3(0, 1, 0));*/
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

        glm::mat4 view;
        if (orbitMode && orbitCamera) {
            orbitCamera->setTarget(boatPosition);
            orbitCamera->update();
            view = orbitCamera->getViewMatrix();
        }
        else {
            // Cam�ra suiveuse par d�faut
            glm::vec3 boatPos = boatPosition;
            float angleRad = glm::radians(boatDirection);
            glm::vec3 offset = glm::vec3(-cos(angleRad) * 10.0f, 4.0f, -sin(angleRad) * 10.0f);
            glm::vec3 cameraPos = boatPos + offset;
            glm::vec3 target = boatPos + glm::vec3(cos(angleRad), 0.0f, sin(angleRad));
            view = glm::lookAt(cameraPos, target, glm::vec3(0, 1, 0));
        }

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

        //Afficher la skybox
        draw_skybox(view, projection);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        scene_root->draw(model, view, projection);


        // Poll for and process events
        glfwPollEvents();

        // flush render commands, and swap draw buffers
        glfwSwapBuffers(win);
    }

    /* close GL context and any other GLFW resources */
    glfwTerminate();
}



void Viewer::key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_key(key, action);
}

void Viewer::on_key(int key, int action)
{
    // 'Q' or 'Escape' quits
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
    {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
    if (animator) {
        int state = glfwGetKey(win, key);
        animator->handleKey(key, state);
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        orbitMode = !orbitMode;
    }
}


void Viewer::draw_skybox(glm::mat4 view, glm::mat4 projection)
{
    glDepthFunc(GL_LEQUAL);      // Important pour bien dessiner le fond
    glDepthMask(GL_FALSE);       // Ne pas �crire dans le z-buffer

    glUseProgram(skyboxShader->get_id());


    glm::mat4 view_no_translation = glm::mat4(glm::mat3(view)); // enlever la position de la cam�ra
    GLuint viewLoc = glGetUniformLocation(skyboxShader->get_id(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_no_translation));

    GLuint projLoc = glGetUniformLocation(skyboxShader->get_id(), "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);        // R�activer l��criture dans le z-buffer
    glDepthFunc(GL_LESS);        // Remettre le comportement normal
}

//assure bon fonctionnement de cam�ra orbitale (les 4 fonctions suivantes)
void Viewer::cursor_position_callback_static(GLFWwindow* window, double xpos, double ypos) {
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        viewer->on_mouse_move(xpos, ypos);
    }
    viewer->lastX = xpos;
    viewer->lastY = ypos;
}

void Viewer::scroll_callback_static(GLFWwindow* window, double xoffset, double yoffset) {
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_scroll(yoffset);
}

void Viewer::on_mouse_move(double xpos, double ypos) {
    double dx = xpos - lastX;
    double dy = ypos - lastY;
    if (orbitCamera) {
        orbitCamera->rotate(-dx * 0.3f, -dy * 0.3f);
    }
    lastX = xpos;
    lastY = ypos;
}

void Viewer::on_scroll(double yoffset) {
    if (orbitCamera) {
        orbitCamera->zoom(-yoffset);
    }
}

void Viewer::mouse_button_callback_static(GLFWwindow* window, int button, int action, int mods) {
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_mouse_button(button, action);
}

void Viewer::on_mouse_button(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        orbitMode = true;
    }
}
