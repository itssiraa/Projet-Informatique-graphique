
#include "shape.h"
#include "shader.h"
#include "texture.h"
#include <GL/glew.h>

class Bords : public Shape {
public:
    Bords::Bords(Shader* shader_program);
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void key_handler(int key);
    virtual ~Bords();
    void addTexture(const std::string& texturePath);
    void Bords::loadTexture(const std::string& path);

private:
    GLuint VAO,EBO,VBO;
    GLuint buffers[2];
    Texture* texture;
    GLuint num_indices; // Ajoutez cette ligne

    std::vector<Texture*> textures;  // Stockage des textures
};


