#include "water.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>
#include <string>
#include "texture.h"
#include "Program.h"
#include "shape.h"
#include <glm/glm.hpp>
#include "TextureLoader.h"


#pragma once

namespace fs = std::filesystem;


// Constante pour le répertoire des textures
#ifndef TEXTURE_DIR
#define TEXTURE_DIR "../textures/"  
#endif

Water::Water(Program* shader_program) : Shape(shader_program), shader_program_(shader_program) {
    // Vérification que GLEW est initialisé après le contexte GLFW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Erreur GLEW: " << glewGetErrorString(err) << std::endl;
        throw std::runtime_error("Échec de l'initialisation de GLEW");
    }
    float size = 100.0f;

    // Configuration du maillage (ajout de coordonnées de texture)
    float vertices[] = {
        // Positions          // TexCoords
          -size, -size,           0.0f, 0.0f,
           size, -size,           1.0f, 0.0f,
          -size,  size,           0.0f, 1.0f,
           size,  size,           1.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Attributs de position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // Attributs de texture
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // Fonction de chargement de texture
    auto loadTexture = [](const std::string& relativePath) -> GLuint {
        fs::path fullPath = fs::path(TEXTURE_DIR) / relativePath;
        std::cout << "Chargement: " << fullPath << std::endl;

        if (!fs::exists(fullPath)) {
            std::cerr << "ERREUR: Fichier non trouvé → " << fullPath << std::endl;
            return 0;
        }

        GLuint texID = 0;
        try {
            TextureLoader::loadTexture(texID, fullPath.string());
        }
        catch (const std::exception& e) {
            std::cerr << "Erreur lors du chargement de texture : " << e.what() << std::endl;
            return 0;
        }

        if (!glIsTexture(texID)) {
            std::cerr << "Texture OpenGL invalide pour " << fullPath << std::endl;
            return 0;
        }

        return texID;
        };

    // Chargement des textures avec vérification
    for (int i = 1; i <= 13; ++i) {
        std::string num = std::to_string(i);
        GLuint height = loadTexture((fs::path("heights") / (num + ".png")).string());
        GLuint normal = loadTexture((fs::path("normals") / (num + ".png")).string());

        if (height) heightMaps.push_back(height);
        if (normal) normalMaps.push_back(normal);
    }

    waterTex = loadTexture("water.jpg");
    if (!waterTex) waterTex = createFallbackTexture();

    wavesHeightMap = loadTexture("wavesHeight.jpg");
    if (!wavesHeightMap) wavesHeightMap = createFallbackTexture();

    wavesNormalMap = loadTexture("wavesNormal.jpg");
    if (!wavesNormalMap) wavesNormalMap = createFallbackTexture();


    glBindVertexArray(0);
}


GLuint Water::createFallbackTexture() {
    GLuint texID;
    glGenTextures(1, &texID);

    // Texture bleue avec dégradé
    unsigned char pixels[3 * 64 * 64];
    for (int i = 0; i < 64 * 64; ++i) {
        pixels[3 * i] = 0;
        pixels[3 * i + 1] = 100 + (i % 64);
        pixels[3 * i + 2] = 255 - (i / 64);
    }

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texID;
}

Water::~Water() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    auto deleteTexture = [](GLuint tex) {
        if (glIsTexture(tex)) glDeleteTextures(1, &tex);
        };

    deleteTexture(waterTex);
    deleteTexture(wavesHeightMap);
    deleteTexture(wavesNormalMap);

    std::for_each(heightMaps.begin(), heightMaps.end(), deleteTexture);
    std::for_each(normalMaps.begin(), normalMaps.end(), deleteTexture);
}

void Water::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {

    if (!waterTex) return;

    glEnable(GL_DEPTH_TEST);       // ← ajoute ceci
    glDepthMask(GL_TRUE);          // ← ajoute ceci
    glDisable(GL_BLEND);           // ← et ceci si blending actif

    float time = static_cast<float>(glfwGetTime());

    shader_program_->use();
    shader_program_->setMat4("model", model);
    shader_program_->setMat4("view", view);
    shader_program_->setMat4("projection", projection);
    shader_program_->setFloat("time", time);
    shader_program_->setFloat("texScale", 0.5f);

    glm::vec3 cameraPos = glm::vec3(glm::inverse(view)[3]);
    shader_program_->setVec3("viewPos", cameraPos);
    shader_program_->setVec3("lightDir", glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f)));

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, waterTex);
    shader_program_->setInt("waterTexture", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, wavesNormalMap);
    shader_program_->setInt("wavesNormalMap", 1);

    glBindVertexArray(VAO);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthMask(GL_TRUE);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}


