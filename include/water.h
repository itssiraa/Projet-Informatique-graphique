
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Program.h"
#include "shape.h"

#pragma once

class Water : public Shape {
public:
    Water(Program* shader_program);
    ~Water();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    Program* shader_program_;
    GLuint VAO, VBO;
    int vertexCount;
    std::vector<GLuint> heightMaps, normalMaps;
    GLuint waterTex, wavesHeightMap, wavesNormalMap;

    // Paramètres d'animation
    unsigned firstIndex = 0, lastIndex = 1;
    float interpolateFactor = 0.0f, offset = 0.0f;
    double lastFrame = 0.0;

    GLuint createFallbackTexture();
    const std::string textureDir = "textures/"; // Chemin relatif
};
