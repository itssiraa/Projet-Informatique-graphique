#version 330 core
in vec3 position;
uniform mat4 mvp;  // Utilisez une matrice MVP combinée
uniform vec3 fragColorUniform;
out vec3 fragColor;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    fragColor = fragColorUniform;  // Passe la couleur uniforme au fragment shader
}