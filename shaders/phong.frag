#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;   // extra in variable, since we need the light position in view space we calculate this in the vertex shader

uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient
    // TODO define ambient strength
    float ambiantStrength = 0.1;
    vec3 ambient = ambiantStrength*lightColor;   // TODO calculate correct ambient color
	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir =normalize(LightPos - FragPos) ;// TODO calculate light direction
    float diff = max(dot(norm,lightDir),0.0); // TODO calculate correct diffuse strength
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.8;
    vec3 viewDir = normalize(-FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),64 );
    vec3 specular =specularStrength*spec*lightColor ;   // TODO calculate corrcet specular color
    
    vec3 result = (diffuse+ specular +ambient)*objectColor; // TODO apply ambient, diffuse and specular to object color
    FragColor = vec4(result, 1.0);
}