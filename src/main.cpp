#include "viewer.h"
#include "bordsBateau.h"
#include "textured_sphere.h"
#include "lighting_sphere.h"
#include "demi_sphere.h"
#include "texture.h"
#include "node.h"
#include "shader.h"
#include "pyramid.h"
#include "cylinder.h"
#include "triangle.h"
#include <string>

#include "BoatAnimator.h"

#include "Program.h"
#include "ShaderUnit.h"
#include "OrbitCamera.h"
#include "TextureLoader.h"
#include "water.h"


#include <iostream>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef SHADER_DIR
#error "SHADER_DIR not defined"
#endif

#ifndef TEXTURE_DIR
#error "TEXTURE_DIR not defined"
#endif



#include "stb_image.h"

#include <iostream>
using namespace std;

unsigned int loadCubemap(vector<std::string> faces)
{
    // Génération et liaison de la texture pour le Skybox
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // Chargement des images de chaque face
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {

        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        std::cout << "Chargement de : " << faces[i] << std::endl;
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    } // Paramètres de texture pour le Skybox
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);



    return textureID;
}

int main()
{

    // Initialisation du Viewer et des shaders
    Viewer viewer;

    // Récupération des chemins vers les répertoires de shaders et textures
    std::string shader_dir = SHADER_DIR;
    std::string texture_dir = TEXTURE_DIR;

    // Initialisation du shader pour les textures
    Shader* texture_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");



    // Initialisation du shader pour le Skybox
    Shader* skyboxShader = new Shader(shader_dir + "skybox.vert", shader_dir + "skybox.frag");

    // Chargement des textures et création des objets

    Texture* texture = new Texture(texture_dir + "bois.jpg");
    Texture* texture1 = new Texture(texture_dir + "bois1.jpg");
    //Shape* sphere1 = new TexturedSphere(texture_shader, texture);

    // Matrice de transformation de la première sphère
    //glm::mat4 sphere1_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -4.0f))
      //  * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    // Création du Node pour l'arbre de scène
    //Node* sphere1_node = new Node(sphere1_mat);
   // sphere1_node->add(sphere1);
    //viewer.scene_root->add(sphere1_node);


    
    // Deuxième sphère avec Phong shading
    Shader* phong_shader = new Shader(shader_dir + "phong.vert", shader_dir + "phong.frag");


   
   // Shape* sphere2 = new LightingSphere(phong_shader, glm::vec3(0.0f, 1.0f, 0.0f),
     //   glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //glm::mat4 sphere2_mat = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, -4.0f))
      //  * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    //Node* sphere2_node = new Node(sphere2_mat);
    //sphere2_node->add(sphere2);
    //viewer.scene_root->add(sphere2_node);



    //bas du bateau 
    Shape* pyramide = new Pyramid(texture_shader);


    
    glm::mat4 coque_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -0.5f, -2.0f));
    

    Node* coque = new Node(coque_mat);
    coque->add(pyramide);

    // dessus bateau
    Shape* pyramide1 = new Bords(texture_shader);

    glm::mat4 coque1_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, -0.01f));
        

        

    Node* coque1 = new Node(coque1_mat);
    coque1->add(pyramide1);



    Shape* pyramide3 = new Bords(texture_shader);

    glm::mat4 coque3_mat = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.5f, -0.5f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    Node* coque3= new Node(coque3_mat);
    coque3->add(pyramide3);

   
    coque->add(coque1);

    coque->add(coque3);

    //mat du bateau 
    glm::mat4 mat_gauche_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.7f, 0.95f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));


    Node* mat_gauche = new Node(mat_gauche_mat);
    coque->add(mat_gauche);

    glm::mat4 mat_DROIT_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.70f, 0.7f, 0.95f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));


    Node* mat_DROIT = new Node(mat_DROIT_mat);
    coque->add(mat_DROIT);


    //Rembardes 
    Shape* rembarde1 = new Cylinder(texture_shader, 1.7, 0.03, 20);

    glm::mat4  rembarde1_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.8f, -0.01f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        


    Node* rembarde1_node = new Node(rembarde1_mat);
    rembarde1_node->add(rembarde1);
    coque->add(rembarde1_node);

    Shape* rembarde2 = new Cylinder(texture_shader, 1.7, 0.03, 20);

    glm::mat4  rembarde2_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.8f, -0.5f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
       


    Node* rembarde2_node = new Node(rembarde2_mat);
    rembarde2_node->add(rembarde2);
    coque->add(rembarde2_node);

    // Barretes rembardes bateau arrière
    Shape* barette1 = new Cylinder(texture_shader, 0.3, 0.03, 20);

    glm::mat4  barette1_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.65f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));



    Node* barette1_node = new Node(barette1_mat);
    barette1_node->add(barette1);
    coque->add(barette1_node);

    Shape* barette2 = new Cylinder(texture_shader, 0.3, 0.03, 20);

    glm::mat4  barette2_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.8f, 0.65f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));



    Node* barette2_node = new Node(barette2_mat);
    barette2_node->add(barette2);
    coque->add(barette2_node);

    Shape* barette3= new Cylinder(texture_shader, 0.3, 0.03, 20);

    glm::mat4  barette3_mat = glm::translate(glm::mat4(1.0f), glm::vec3(1.1f, 0.65f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));



    Node* barette3_node = new Node(barette3_mat);
    barette3_node->add(barette3);
    coque->add(barette3_node);


    Shape* barette4 = new Cylinder(texture_shader, 0.3, 0.03, 20);

    glm::mat4  barette4_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, 0.65f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


    //premières
    Node* barette4_node = new Node(barette4_mat);
    barette4_node->add(barette4);
    coque->add(barette4_node);



    Shape* barette5 = new Cylinder(texture_shader, 0.3, 0.03, 20);

    glm::mat4  barette5_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f, 0.65f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


    
    Node* barette5_node = new Node(barette5_mat);
    barette5_node->add(barette5);
    coque->add(barette5_node);



    Shape* barette6 = new Cylinder(texture_shader, 0.3, 0.03, 20);

    glm::mat4  barette6_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.4f, 0.65f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));



    Node* barette6_node = new Node(barette6_mat);
    barette6_node->add(barette6);
    coque->add(barette6_node);




   

    
    // Barretes rembardes bateau avant 
    Shape* barette7 = new Cylinder(texture_shader, 0.3, 0.03, 20);

    glm::mat4  barette7_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.65f, -0.5f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));



    Node* barette7_node = new Node(barette7_mat);
    barette7_node->add(barette7);
    coque->add(barette7_node);

    //////////////////////////////////

    Shape* barette8 = new Cylinder(texture_shader, 0.3, 0.03, 20);

    glm::mat4  barette8_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.8f, 0.65f, -0.5f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));



    Node* barette8_node = new Node(barette8_mat);
    barette8_node->add(barette8);
    coque->add(barette8_node);

    ///////////////////////////////

    Shape* barette9 = new Cylinder(texture_shader, 0.3, 0.03, 20);

    glm::mat4  barette9_mat = glm::translate(glm::mat4(1.0f), glm::vec3(1.1f, 0.65f, -0.5f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));



    Node* barette9_node = new Node(barette9_mat);
    barette9_node->add(barette9);
    coque->add(barette9_node);

    //////////////////////////

    Shape* barette10 = new Cylinder(texture_shader, 0.3, 0.03, 20);

    glm::mat4  barette10_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, 0.65f, -0.5f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


    //premières
    Node* barette10_node = new Node(barette10_mat);
    barette10_node->add(barette10);
    coque->add(barette10_node);

    //////////////////////////

    Shape* barette11 = new Cylinder(texture_shader, 0.3, 0.03, 20);

    glm::mat4  barette11_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f, 0.65f, -0.5f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));



    Node* barette11_node = new Node(barette11_mat);
    barette11_node->add(barette11);
    coque->add(barette11_node);

    ////////////////////////////

    Shape* barette12 = new Cylinder(texture_shader, 0.3, 0.03, 20);

    glm::mat4  barette12_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.4f, 0.65f, -0.5f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));



    Node* barette12_node = new Node(barette12_mat);
    barette12_node->add(barette12);
    coque->add(barette12_node);



    //haut du bateau vertical
    Shape* cylindre = new Cylinder(texture_shader, 3,0.05, 20 );

    glm::mat4 cylindre_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, -4.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
       

    Node* batton = new Node(cylindre_mat);
    batton->add(cylindre);
    mat_gauche->add(batton);

    /////////////////////// vertical

    Shape* cylindre2 = new Cylinder(texture_shader, 3, 0.05, 20);

    glm::mat4 cylindre2_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 1.0f,-4.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


    Node* batton2 = new Node(cylindre2_mat);
    batton2->add(cylindre2);
    mat_DROIT->add(batton2);

    // Cylindres horizontaux
    //////////////////////////////// horizontale haut droite
    

    Shape* cylindre3 = new Cylinder(texture_shader, 2.5, 0.05, 20);

    glm::mat4 cylindre3_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 1.7f, -4.0f));
       


    Node* batton3 = new Node(cylindre3_mat);
    batton3->add(cylindre3);
    mat_DROIT->add(batton3);
  

    //////////////////////////////// horizontale haut gauche


    Shape* cylindre4 = new Cylinder(texture_shader, 2.5, 0.05, 20);

    glm::mat4 cylindre4_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.7f, -4.0f));



    Node* batton4 = new Node(cylindre4_mat);
    batton4->add(cylindre4);
    mat_gauche->add(batton4);
    

    ////////////////////////////////demi sphère haute gauche


    Shape* demi_sphere1 = new Demi_Sphere(texture_shader);

    glm::mat4 demi_sphere1_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.9f, -4.0f))
        * glm::scale(glm::mat4(0.5f), glm::vec3(0.3f, 0.3f, 0.3f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));



    Node* demi_sphere1_node = new Node(demi_sphere1_mat);
    demi_sphere1_node->add(demi_sphere1);
    mat_gauche->add(demi_sphere1_node);


    ////////////////////////////////demi sphère haute droite


    Shape* demi_sphere2 = new Demi_Sphere(texture_shader, 36, 18);

    glm::mat4 demi_sphere2_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 1.9f, -4.0f))
        * glm::scale(glm::mat4(0.5f), glm::vec3(0.3f, 0.3f, 0.3f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));



    Node* demi_sphere2_node = new Node(demi_sphere2_mat);
    demi_sphere2_node->add(demi_sphere2);
    mat_DROIT ->add(demi_sphere2_node);
    ////////////////////////////////cylindre bas droite


    Shape* cylindre5 = new Cylinder(texture_shader, 2, 0.05, 20);

    glm::mat4 cylindre5_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.25f, -4.0f));



    Node* batton5 = new Node(cylindre5_mat);
    batton5->add(cylindre5);
    mat_DROIT->add(batton5);
    

    ////////////////////////////////cylindre bas gauche


    Shape* cylindre6 = new Cylinder(texture_shader, 2, 0.05, 20);

    glm::mat4 cylindre6_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.25f, -4.0f));



    Node* batton6 = new Node(cylindre6_mat);
    batton6->add(cylindre6);
    mat_gauche->add(batton6);
    

    ////////////////////////////////bas gauche


    Shape* demi_sphere4 = new Demi_Sphere(texture_shader);

    glm::mat4 demi_sphere4_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.35f, -4.0f))
        * glm::scale(glm::mat4(0.5f), glm::vec3(0.3f, 0.3f, 0.3f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));



    Node* demi_sphere4_node = new Node(demi_sphere4_mat);
    demi_sphere4_node->add(demi_sphere4);
    mat_gauche->add(demi_sphere4_node);


    ////////////////////////////////bas droite


    Shape* demi_sphere5 = new Demi_Sphere(texture_shader, 36, 18);

    glm::mat4 demi_sphere5_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.35f, -4.0f))
        * glm::scale(glm::mat4(0.5f), glm::vec3(0.3f, 0.3f, 0.3f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));



    Node* demi_sphere5_node = new Node(demi_sphere5_mat);
    demi_sphere5_node->add(demi_sphere5);
    mat_DROIT->add(demi_sphere5_node);


    //Voiles du bateau
     Shader* shader1 = new Shader(shader_dir + "color.vert", shader_dir + "color.frag");
    ///////////////////haut gauche
     Shape* triangle1 = new Triangle(shader1);

     glm::mat4 voile1_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 2.7f, -4.0f))
         * glm::scale(glm::mat4(0.5f), glm::vec3(1.5f, 1.5f, 1.5f))
         * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));


     Node* triangle1_node = new Node(voile1_mat);
     triangle1_node->add(triangle1);

     mat_gauche->add(triangle1_node);

    


    /////////////////////bas gauche
     Shape* triangle2 = new Triangle(shader1);

     glm::mat4 voile2_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, -4.0f))
         * glm::scale(glm::mat4(0.5f), glm::vec3(1.2f, 1.2f, 1.2f))
         * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))
         * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

     Node* triangle2_node = new Node(voile2_mat);
     triangle2_node->add(triangle2);

     mat_gauche->add(triangle2_node);
   


    ///////////////////haut droite
    Shape* triangle3 = new Triangle(shader1);

    glm::mat4 voile3_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 2.7f, -4.0f))
        * glm::scale(glm::mat4(0.5f), glm::vec3(1.5f, 1.5f, 1.5f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    Node* triangle3_node = new Node(voile3_mat);
    triangle3_node->add(triangle3);

    mat_DROIT->add(triangle3_node);
   

    ///////////////////bas droite
    Shape* triangle4 = new Triangle(shader1);

    glm::mat4 voile4_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 1.0f, -4.0f))
        * glm::scale(glm::mat4(0.5f), glm::vec3(1.2f, 1.2f, 1.2f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* triangle4_node = new Node(voile4_mat);
    triangle4_node->add(triangle4);

    mat_DROIT->add(triangle4_node);


    viewer.boatTransformNode = new Node();
    viewer.boatTransformNode->add(coque);  // ou ton Node principal du bateau
    
    BoatAnimator* animator = new BoatAnimator(viewer.boatTransformNode);
    animator->setSpeed(0.5f); // optionnel

    viewer.setAnimator(animator);

 



    //viewer.scene_root->add(coque);



    
    
    //Skybox
    /*j'ai créé une nouvelle fonction dans le viewer.cpp qui permet de dessiner le skybox et donc son prototype est donné comme champs dans le fichier viewer.h
    j'ai modifier le viewer.run() pour qu'il inclue l'affichage de la skybox en appelant dedans la fonction créée précédemment */
    //Sommets du cubes 
    float skyboxVertices[] = {
        // positions          
           -1.0f,  1.0f, -1.0f,
           -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
           -1.0f,  1.0f, -1.0f,

           -1.0f, -1.0f,  1.0f,
           -1.0f, -1.0f, -1.0f,
           -1.0f,  1.0f, -1.0f,
           -1.0f,  1.0f, -1.0f,
           -1.0f,  1.0f,  1.0f,
           -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

           -1.0f, -1.0f,  1.0f,
           -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
           -1.0f, -1.0f,  1.0f,

           -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
           -1.0f,  1.0f,  1.0f,
           -1.0f,  1.0f, -1.0f,

           -1.0f, -1.0f, -1.0f,
           -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
           -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };



    



    // Définition des différentes faces du Skybox
    vector<std::string> faces = {
            texture_dir + "right.jpg",
            texture_dir + "left.jpg",
            texture_dir + "top.jpg",
            texture_dir + "bottom.jpg",
            texture_dir + "front.jpg",
            texture_dir + "back.jpg"
    };

    unsigned int cubemapTexture = loadCubemap(faces);
    if (cubemapTexture == 0) {
        std::cerr << "Erreur lors du chargement du Skybox !" << std::endl;
        return -2;
    }



  
    // Rendu du Skybox
    
    // Génération et liaison du VAO pour le Skybox
    GLuint skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);

    //Génération et liaison du VBO pour le Skybox
    GLuint skyboxVBO;
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);  // On délie le VAO

    // Activation manuelle du shader skybox et transmission de la texture cubemap à l'uniforme
    viewer.skyboxShader = skyboxShader;
    viewer.skyboxVAO = skyboxVAO;
    viewer.cubemapTexture = cubemapTexture;


    // Lancement de la boucle principale

    // Version sans tessellation (adaptée pour GLSL 330 core)
    
    Program* waterProgram = new Program();
    waterProgram->create();
    
    waterProgram->attachShader(ShaderUnit::createVertexShader(shader_dir + "water.vert"));
    waterProgram->attachShader(ShaderUnit::createFragmentShader(shader_dir + "water.frag"));

    try {
        waterProgram->link();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Erreur de linking: " << e.what() << std::endl;
        exit(-1);
    }

    glm::mat4 waterTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.6f, -5.0f));

    Node* waterNode = new Node(waterTransform);
    Shape* water = new Water(waterProgram);  // Doit utiliser la version modifiée de Water.cpp
    waterNode->add(water);

    viewer.scene_root->add(waterNode);
    viewer.scene_root->add(viewer.boatTransformNode);

    

    viewer.run();


}