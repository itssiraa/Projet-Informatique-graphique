#version 330 core

in vec2 texCoords;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D waterTexture;
uniform sampler2D wavesNormalMap;
uniform vec3 lightDir;  // direction vers la lumière (non inversée)
uniform vec3 viewPos;

void main()
{
    // 1. Normale depuis la normal map (convertie de [0,1] à [-1,1])
    vec3 normal = texture(wavesNormalMap, texCoords).rgb;
    normal = normal * 2.0 - 1.0;
    if (length(normal) < 0.1)
        normal = vec3(0.0, 1.0, 0.0); // normale vers le haut par défaut
    normal = normalize(normal);

    // 2. Calculs de lumière
    vec3 light = normalize(-lightDir); // la lumière vient de cette direction
    vec3 viewDir = normalize(viewPos - fragPos); // direction vers la caméra

    // 3. Diffuse (Lambert)
    float diff = max(dot(normal, light), 0.0);

    // 4. Spéculaire (Blinn-Phong)
    vec3 halfwayDir = normalize(light + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    // 5. Couleur de base de l'eau
    vec3 tex = texture(waterTexture, texCoords).rgb;
    vec3 base = mix(tex, vec3(0.0, 0.55, 0.8), 0.5);

    // 6. Couleur finale
    vec3 ambient = base * 0.4;                // lumière ambiante douce
    vec3 diffuse = base * diff * 0.6;         // lumière diffuse
    vec3 specular = vec3(1.0) * spec * 0.2;   // reflet blanc

    vec3 finalColor = ambient + diffuse + specular;

    fragColor = vec4(finalColor, 0.8);
}
