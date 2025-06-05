#pragma once
 
#include "shape.h"
#include "shader.h"
#include <vector>
#include <glm/glm.hpp>

#ifndef TEXTURED_SPHERE_H
#define TEXTURED_SPHERE_H
#endif 

#include "sphere.h"
#include "texture.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


class Demi_Sphere : public Shape {
public:
    Demi_Sphere(Shader* shader_program_, int sector_count = 36, int stack_count = 18);

    virtual void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);

    void key_handler(int key);


private:

    void buildVertices();
    void buildIndices();
 

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    int sector_count;
    int stack_count;

    unsigned int num_indices;

    GLuint VAO;
    GLuint buffers[3];


    std::vector<glm::vec2> tex_coords;
    Texture* texture;
    GLint loc_diffuse_map;

  

};
